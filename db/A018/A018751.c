/*
 * A018751  Divisors of 970.
 * https://oeis.org/A018751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 970, sorted ascending. */
#include <stdlib.h>

/* Array to hold the divisors of 970 (8 divisors). */
static seq_t divisors[8];
/* Number of divisors found. */
static size_t num_divisors = 0;
/* Flag indicating whether the divisor table has been built. */
static int initialized = 0;

/* Comparison function for qsort over seq_t. */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Recursive generation of all divisors.
   depth   : current prime being processed.
   primes, exps : arrays of primes and their exponents.
   pcnt    : number of distinct prime factors.
   cur     : current divisor being built.
   arr     : array to store the divisors.
   pos     : pointer to the next free position in arr. */
static void gen_rec(size_t depth,
                    uint64_t *primes,
                    uint8_t *exps,
                    size_t pcnt,
                    seq_t cur,
                    seq_t *arr,
                    size_t *pos)
{
    if (depth == pcnt) {
        arr[*pos] = cur;
        (*pos)++;
        return;
    }
    uint64_t p = primes[depth];
    seq_t prod = cur;   /* exponent 0 */
    gen_rec(depth + 1, primes, exps, pcnt, prod, arr, pos);
    for (uint8_t e = 1; e <= exps[depth]; ++e) {
        prod = prod * (seq_t)p;
        gen_rec(depth + 1, primes, exps, pcnt, prod, arr, pos);
    }
}

/* Build the divisor table for 970 on first use. */
static void init_divisors(void)
{
    uint64_t n = 970;
    uint64_t primes[10];
    uint8_t exps[10];
    size_t pcnt = 0;

    /* Simple trial division to factor 970. */
    uint64_t cand = 2;
    while (cand * cand <= n) {
        if (n % cand == 0) {
            primes[pcnt] = cand;
            exps[pcnt] = 0;
            while (n % cand == 0) {
                n /= cand;
                exps[pcnt] += 1;
            }
            pcnt++;
        }
        cand++;
    }
    if (n > 1) {
        primes[pcnt] = n;
        exps[pcnt] = 1;
        pcnt++;
    }

    /* Total number of divisors = product (exps + 1). */
    size_t total = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        total *= (size_t)(exps[i] + 1);
    }
    num_divisors = total;

    /* Generate all divisors into the static array. */
    size_t pos = 0;
    gen_rec(0, primes, exps, pcnt, (seq_t)1, divisors, &pos);

    /* Sort them in ascending order. */
    qsort(divisors, total, sizeof(seq_t), cmp_seq_t);

    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_divisors();
    }
    if (n == 0) return (seq_t)0;
    if ((idx_t)num_divisors >= n) {
        return divisors[(size_t)(n - 1)];
    }
    return (seq_t)0;
}
