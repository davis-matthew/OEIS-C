/*
 * A151715  Divisors of 8855.
 * https://oeis.org/A151715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the n-th divisor of 8855 in ascending order
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* The divisor set is generated from the prime factorization of 8855. */
static seq_t divisors[16];
static size_t div_count = 0;
static int initialized = 0;

/* Recursive helper to build all divisors from the prime list. */
static void gen_divisors(const uint64_t *primes, const int *exps,
                         int idx, int count, seq_t prod)
{
    if (idx == count) {
        divisors[div_count++] = prod;
        return;
    }
    /* Multiply by p^e for e = 0 .. exps[idx] */
    seq_t cur = prod;
    for (int e = 0; e <= exps[idx]; ++e) {
        gen_divisors(primes, exps, idx + 1, count, cur);
        cur = cur * (seq_t)primes[idx];
    }
}

/* Simple insertion sort for the small number of divisors. */
static void sort_divisors(void)
{
    for (size_t i = 1; i < div_count; ++i) {
        seq_t key = divisors[i];
        size_t j = i;
        while (j > 0 && divisors[j - 1] > key) {
            divisors[j] = divisors[j - 1];
            --j;
        }
        divisors[j] = key;
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        /* Factor 8855 = 5 * 7 * 11 * 23 */
        const uint64_t target = 8855ULL;
        uint64_t primes[10];
        int exps[10];
        int cnt = 0;
        uint64_t m = target;
        /* Trial division */
        for (uint64_t p = 2; p * p <= m; ++p) {
            if (m % p == 0) {
                primes[cnt] = p;
                exps[cnt] = 0;
                while (m % p == 0) {
                    m /= p;
                    ++exps[cnt];
                }
                ++cnt;
            }
        }
        if (m > 1) {
            primes[cnt] = m;
            exps[cnt] = 1;
            ++cnt;
        }

        /* Generate all divisors */
        gen_divisors(primes, exps, 0, cnt, (seq_t)1);
        sort_divisors();
        initialized = 1;
    }

    if (n == 0 || n > div_count) {
        return (seq_t)0;
    }
    return divisors[(size_t)(n - 1)];
}
