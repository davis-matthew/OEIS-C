/*
 * A176509  Composite numbers m for which A064380(m) = A000010(m).
 * https://oeis.org/A176509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sequence of composite numbers m such that A064380(m) =
 *            A000010(m). The numbers are all prime powers p^(2^k-1) with k ≥
 *            2, which are sorted in increasing order. Thus a(n) is the n‑th
 *            smallest number of that form
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 269, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint128_overflow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_uint128_overflow.h"

/* --------------------------------------------------------------------------
   Sieve of Eratosthenes, executed only once.
   -------------------------------------------------------------------------- */
#define PRIME_LIMIT 1000000   /* enough for k up to ~10000 */

static uint32_t *prime_list = NULL;      /* array of primes */
static size_t prime_cnt   = 0;           /* number of primes found */

static void init_primes(void)
{
    if (prime_list != NULL)
        return;          /* already initialized */

    unsigned char *sieve = (unsigned char *)calloc(PRIME_LIMIT + 1, 1);
    prime_list = (uint32_t *)malloc((PRIME_LIMIT + 1) * sizeof(uint32_t));

    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            prime_list[cnt++] = (uint32_t)i;
            if (i * i <= PRIME_LIMIT) {
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                    sieve[j] = 1;
            }
        }
    }
    free(sieve);

    prime_cnt = cnt;
}

/* --------------------------------------------------------------------------
   Main function computing a(n).
   -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* undefined for n < 1, return 0 as placeholder */

    /* one-time prime initialization */
    init_primes();

    /* maximum representable value of seq_t, used for overflow checks */
    __uint128_t max_seq = (__uint128_t)(~(seq_t)0);

    /* exponents e = 2^k - 1 with k >= 2, up to the max allowed by seq_t width */
    int e_vals[10];
    size_t e_cnt = 0;
    int e = 3;
    int max_exp = ((int)sizeof(seq_t) * 8) - 1;  /* 2^e <= max_seq => e <= width-1 */
    while (e <= max_exp) {
        e_vals[e_cnt++] = e;
        e = e * 2 + 1;          /* next exponent in the sequence */
    }

    /* node structure holds current value, its exponent, and current prime index */
    typedef struct {
        seq_t val;          /* current prime power value */
        size_t prime_idx;   /* index into prime_list */
        int exp;            /* exponent of this power */
    } node_t;

    node_t *nodes = (node_t *)malloc(e_cnt * sizeof(node_t));

    /* initialise nodes with the first prime (2) for each exponent */
    for (size_t i = 0; i < e_cnt; ++i) {
        __uint128_t tmp;
        bool ok = pow_uint128_overflow(prime_list[0], e_vals[i], max_seq, &tmp);
        /* this must succeed because 2^e <= max_seq for all exponents we keep */
        nodes[i].val = (seq_t)tmp;
        nodes[i].prime_idx = 0;
        nodes[i].exp = e_vals[i];
    }

    seq_t result = 0;          /* value to return */

    /* generate terms until we reach the n-th one */
    for (idx_t cnt = 1; cnt <= n; ++cnt) {
        /* find node with minimal value */
        size_t min_pos = 0;
        for (size_t i = 1; i < e_cnt; ++i) {
            if (nodes[i].val < nodes[min_pos].val)
                min_pos = i;
        }

        result = nodes[min_pos].val;   /* this is the next term */

        /* advance that node to the next prime */
        size_t next_idx = nodes[min_pos].prime_idx + 1;
        if (next_idx < prime_cnt) {
            __uint128_t next_val;
            bool ok = pow_uint128_overflow(prime_list[next_idx],
                                           nodes[min_pos].exp,
                                           max_seq, &next_val);
            if (ok) {
                nodes[min_pos].prime_idx = next_idx;
                nodes[min_pos].val = (seq_t)next_val;
            } else {
                /* this prime gives an overflow, mark the node as exhausted */
                nodes[min_pos].prime_idx = next_idx;
                nodes[min_pos].val = (seq_t)-1;   /* maximal seq_t value */
            }
        } else {
            /* no more primes for this exponent */
            nodes[min_pos].prime_idx = next_idx;
            nodes[min_pos].val = (seq_t)-1;
        }
    }

    free(nodes);
    return result;
}
