/*
 * A117552  Largest partial sum of the increasingly ordered divisors of n, not
 *          exceeding n.
 * https://oeis.org/A117552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 274, u16 n <= 9926, u32 n <= 9926, u64 n <= 9926,
 *            u128 n <= 9926, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest partial sum of the increasingly ordered divisors
   of n that does not exceed n.  */
#include <stdlib.h>
#include <stdint.h>

/* Comparator for qsort on uint64_t values. */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    return (*x > *y) - (*x < *y);
}

/* Recursively generate all divisors from the prime factorisation of n. */
static void gen_divs_rec(const uint64_t *primes,
                         const uint32_t *exps,
                         size_t cnt,
                         size_t idx,
                         uint64_t cur,
                         uint64_t *out,
                         size_t *len)
{
    if (idx == cnt) {
        out[*len] = cur;
        (*len)++;
        return;
    }
    for (uint32_t e = 0; e <= exps[idx]; ++e) {
        gen_divs_rec(primes, exps, cnt, idx + 1, cur, out, len);
        cur *= primes[idx];
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* outside the offset */

    /* Factorise n. */
    uint64_t temp = (uint64_t)n;
    uint64_t primes[20];
    uint32_t exps[20];
    size_t cnt = 0;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint32_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (temp > 1) {
        primes[cnt] = temp;
        exps[cnt] = 1;
        ++cnt;
    }

    /* Number of divisors. */
    size_t num_divs = 1;
    for (size_t i = 0; i < cnt; ++i)
        num_divs *= (size_t)(exps[i] + 1);

    /* Generate all divisors. */
    uint64_t *divs = (uint64_t *)malloc(num_divs * sizeof(uint64_t));
    if (!divs)
        return (seq_t)0;   /* allocation failure (unlikely) */
    size_t len = 0;
    gen_divs_rec(primes, exps, cnt, 0, 1, divs, &len);

    /* Sort divisors increasingly. */
    qsort(divs, len, sizeof(uint64_t), cmp_uint64);

    /* Find the largest partial sum not exceeding n. */
    seq_t sum = 0;
    seq_t best = 0;
    seq_t limit = (seq_t)n;

    for (size_t i = 0; i < len; ++i) {
        seq_t next = sum + (seq_t)divs[i];
        if (next <= limit) {
            sum = next;
            best = sum;
        } else
            break;
    }

    free(divs);
    return best;
}
