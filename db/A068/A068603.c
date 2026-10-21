/*
 * A068603  Numbers n such that prime(n)+prime(2n)+prime(3n) is prime.
 * https://oeis.org/A068603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth integer k for which prime(k)+prime(2k)+prime(3k)
 *            is prime (search/ sieve approach)
 * Range:     u8 n <= 71, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/*------------------------------------------------------------------*/
/* Prime sieve up to a fixed limit (1 000 000) – sufficient for n up to 26000 */
static uint64_t *primes_global   = NULL;   /* array of primes (0‑based index) */
static size_t   primes_global_cnt = 0;       /* number of primes stored        */

static void init_primes_global(void)
{
    if (primes_global != NULL)
        return;                              /* already initialized */

    const int LIMIT   = 1000000;              /* generate primes ≤ 1e6   */
    bool *mark = (bool*)calloc(LIMIT + 1, sizeof(bool));
    if (!mark)
        return;                              /* allocation failed – abort */

    size_t cap  = 100000;                    /* initial allocation size */
    uint64_t *pr = (uint64_t*)malloc(cap * sizeof(uint64_t));
    size_t   cnt = 0;

    /* classic sieve of Eratosthenes */
    for (int i = 2; i * i <= LIMIT; ++i)
        if (!mark[i])
            for (int j = i * i; j <= LIMIT; j += i)
                mark[j] = true;

    for (int i = 2; i <= LIMIT; ++i) {
        if (!mark[i]) {
            if (cnt == cap) {                /* grow array if needed */
                cap <<= 1;
                pr = (uint64_t*)realloc(pr, cap * sizeof(uint64_t));
            }
            pr[cnt++] = (uint64_t)i;
        }
    }

    free(mark);
    primes_global       = pr;
    primes_global_cnt   = cnt;
}

/* Simple primality test for the sum, using the primes array as trial divisors */
static bool sum_is_prime(uint64_t val)
{
    if (val < 2)
        return false;
    for (size_t i = 0; i < primes_global_cnt; ++i) {
        uint64_t p = primes_global[i];
        if (p * p > val)
            break;
        if (val % p == 0)
            return false;
    }
    return true;
}

/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Static storage for sequence data */
    static seq_t *seq       = NULL;   /* array of found terms  */
    static size_t seq_len   = 0;      /* number of terms stored */
    static size_t seq_cap   = 0;      /* capacity of seq array  */
    static idx_t next_candidate = 1;  /* next integer to test   */

    /* Ensure primes are ready */
    init_primes_global();

    /* Generate terms until we have at least n of them */
    while (seq_len < n) {
        /* Need primes up to index 3*next_candidate (1‑based) */
        size_t needed = 3 * next_candidate;
        if (needed > primes_global_cnt)
            return 0; /* insufficient primes – should not happen with limit */

        uint64_t sum = primes_global[next_candidate - 1]       /* prime(n)  */
                     + primes_global[2 * next_candidate - 1]  /* prime(2n) */
                     + primes_global[3 * next_candidate - 1];/* prime(3n) */

        if (sum_is_prime(sum)) {
            /* Append to sequence */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return seq[n - 1];
}
