/*
 * A018732  Divisors of 938.
 * https://oeis.org/A018732
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

/* Sequence of all positive divisors of 938, sorted in increasing order. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t divs[64];
static size_t div_cnt = 0;
static bool initialized = false;

/* Recursive helper to generate divisors */
static void rec_divisors(int idx, seq_t cur,
                         const uint64_t *primes, const int *exps,
                         int pf, seq_t *out, size_t *cnt)
{
    if (idx == pf) {
        out[*cnt] = cur;
        ++*cnt;
        return;
    }
    seq_t prod = cur;
    for (int e = 0; e <= exps[idx]; ++e) {
        rec_divisors(idx + 1, prod, primes, exps, pf, out, cnt);
        prod *= (seq_t)primes[idx];
    }
}

static void gen_divs(void)
{
    uint64_t m = (uint64_t)938;
    uint64_t primes[64];
    int exps[64];
    int pf = 0;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[pf] = p;
            exps[pf] = e;
            ++pf;
        }
    }
    if (m > 1) {
        primes[pf] = m;
        exps[pf] = 1;
        ++pf;
    }

    rec_divisors(0, (seq_t)1, primes, exps, pf, divs, &div_cnt);

    /* Simple insertion sort (div_cnt is tiny, < 10) */
    for (size_t i = 1; i < div_cnt; ++i) {
        seq_t key = divs[i];
        int j = (int)i - 1;
        while (j >= 0 && divs[j] > key) {
            divs[j + 1] = divs[j];
            --j;
        }
        divs[j + 1] = key;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        gen_divs();
    }
    if (n == 0) return (seq_t)0;
    size_t idx = (size_t)n - 1;
    if (idx >= div_cnt) return (seq_t)0;
    return divs[idx];
}
