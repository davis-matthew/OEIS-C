/*
 * A071621  Primes that can be written as "a * b + c * d", where a, b, c and d
 *          are also primes.
 * https://oeis.org/A071621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 6536, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A071621: primes that can be written as a*b + c*d
 * where a, b, c, and d are also primes.  The function is memoised
 * so that arbitrary order of index requests is handled correctly. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* -------------------------------------------------------------------- */
/*  Global data for the sieve and for caching the sequence              */
/* -------------------------------------------------------------------- */
static uint64_t *prime_list     = NULL;   /* prime list up to current limit         */
static size_t   prime_cnt       = 0;      /* number of primes in prime_list         */
static uint8_t  *prime_flag     = NULL;   /* 0 = prime, 1 = composite               */
static size_t   flag_limit      = 0;      /* current size of prime_flag array       */

static seq_t   *seq_vals        = NULL;   /* array of representable primes          */
static size_t   seq_cap         = 0;      /* capacity of seq_vals                   */
static size_t   seq_cnt         = 0;      /* number of values stored                */
static size_t   next_p_idx      = 0;      /* next prime index to inspect            */

/* -------------------------------------------------------------------- */
/*  Helper: sieve primes up to 'limit' (inclusive).  The array
 *  'prime_flag' is 0‑initialised for primes, 1 for composites.
 *  The prime list 'prime_list' is cleared and filled with all
 *  primes up to 'limit'.  The function may be called multiple
 *  times; it only enlarges the sieve if 'limit' is greater than
 *  the current 'flag_limit'.                                      */
/* -------------------------------------------------------------------- */
static void
init_sieve(uint64_t limit)
{
    if (limit <= flag_limit) return;          /* already big enough */

    /* allocate the flag array (zero = prime, one = composite)   */
    uint8_t *new_flag = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!new_flag) return;                    /* OOM – should not happen */

    /* Eratosthenes' sieve.  0 means prime, 1 composite. */
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!new_flag[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                new_flag[j] = 1;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!new_flag[i]) ++cnt;

    /* rebuild the prime list */
    uint64_t *new_list = (uint64_t *)realloc(prime_list, cnt * sizeof(uint64_t));
    size_t pos = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!new_flag[i]) new_list[pos++] = i;

    /* replace old data */
    free(prime_flag);
    prime_flag = new_flag;
    prime_list = new_list;
    prime_cnt  = cnt;
    flag_limit = limit;
}

/* -------------------------------------------------------------------- */
/*  Test whether p can be expressed as a*b + c*d with prime a,b,c,d.   */
/* -------------------------------------------------------------------- */
static bool
representable(uint64_t p)
{
    if (p < 13) return false;                /* first representable prime is 13 */

    for (size_t ia = 0; ia < prime_cnt; ++ia) {
        uint64_t a = prime_list[ia];
        if (a > p) break;
        if (a * a > p) break;                /* a ≤ √p */

        for (size_t ib = ia; ib < prime_cnt; ++ib) {
            uint64_t b = prime_list[ib];
            uint64_t ab = a * b;
            if (ab >= p) break;              /* a*b grows monotonically in b */

            uint64_t r = p - ab;             /* remaining part must be c*d */

            /* test whether r is product of two primes */
            for (size_t ic = 0; ic < prime_cnt; ++ic) {
                uint64_t c = prime_list[ic];
                if (c > r) break;
                if (c * c > r) break;        /* c ≤ √r */

                if (r % c == 0) {
                    uint64_t d = r / c;
                    if (!prime_flag[d])     /* d is prime (prime_flag=0) */
                        return true;
                }
            }
        }
    }
    return false;
}

/* -------------------------------------------------------------------- */
/*  Main sequence function                                              */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* behavior for n==0 is irrelevant */

    /* initial sieve limit – large enough for the first 10 000 terms.
       5 000 000 gives more than 150 000 representable primes. */
    if (flag_limit == 0)
        init_sieve(5000000);

    /* initialise the result array lazily */
    if (!seq_vals) {
        seq_cap = 16;
        seq_vals = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    /* generate terms until we reach the requested index 'n' */
    size_t target = (size_t)n;
    while (seq_cnt < target) {
        /* extend sieve only if we have run out of primes */
        if (next_p_idx >= prime_cnt) {
            /* double the sieve limit */
            init_sieve(flag_limit * 2);
            /* next_p_idx stays the same because the list order is preserved */
        }

        uint64_t p = prime_list[next_p_idx++];
        if (representable(p)) {
            if (seq_cnt >= seq_cap) {
                seq_cap *= 2;
                seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
            }
            seq_vals[seq_cnt++] = (seq_t)p;
        }
    }

    return seq_vals[target - 1];
}
