/*
 * A225817  Moebius function applied to divisors of n, table read by rows.
 * https://oeis.org/A225817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) = flatten_rows_of_mu_of_divisors(n)
 * Range:     i8 n <= 7069, i16 n <= 7069, i32 n <= 7069, i64 n <= 7069,
 *            i128 n <= 7069, big any n
 * Helpers:   divisor_count_u64, factor_u64, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, a(n) is the n-th entry in the row-major list of the Möbius
 * values of all divisors of every positive integer, taken row by row.
 * Row m consists of μ(d) for each divisor d of m (sorted in increasing order). */

#include <stdlib.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/factor_u64.h"
#include "../utils/mobius.h"

/* Comparator for qsort on idx_t */
static int cmp_idx_t(const void *a, const void *b)
{
    idx_t x = *(const idx_t *)a;
    idx_t y = *(const idx_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

/* Recursive generation of all divisors of a number from its prime factorisation.
 * `primes` and `exps` contain the prime factors and their multiplicities,
 * `cnt` is the number of distinct primes.
 * The current divisor is carried in `cur`.  All divisors are stored
 * in the `divs` array; `div_cnt` collects the number generated.
 */
static void gen_divs(const uint64_t *primes, const uint64_t *exps, size_t cnt,
                     size_t pos, idx_t cur, idx_t *divs, size_t *div_cnt)
{
    if (pos == cnt) {
        divs[*div_cnt] = cur;
        ++(*div_cnt);
        return;
    }
    idx_t val = 1;
    for (size_t e = 0; e <= exps[pos]; ++e) {
        gen_divs(primes, exps, cnt, pos + 1, cur * val, divs, div_cnt);
        val *= (idx_t)primes[pos];
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n=0 but harmless here */

    /* Find the row that contains the n-th entry */
    idx_t target = n;
    idx_t sum = 0;
    idx_t row = 0;
    idx_t tau = 0;

    while (1) {
        ++row;
        tau = divisor_count_u64((uint64_t)row);
        if (sum + tau >= target) break;
        sum += tau;
    }

    idx_t k = target - sum; /* 1-based index within the row */

    /* Factorise the row number */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t prime_cnt = 0;
    factor_u64((uint64_t)row, primes, exps, &prime_cnt);

    /* Generate all divisors of the row number */
    idx_t *divs = (idx_t *)malloc((size_t)tau * sizeof(idx_t));
    if (!divs) return 0; /* allocation failure fallback */
    size_t div_cnt = 0;
    gen_divs(primes, exps, prime_cnt, 0, (idx_t)1, divs, &div_cnt);

    /* Sort divisors to obtain the k-th in ascending order */
    qsort(divs, tau, sizeof(idx_t), cmp_idx_t);

    idx_t divisor = divs[k - 1];
    free(divs);

    /* Return the Möbius value of that divisor */
    return mobius(divisor);
}
