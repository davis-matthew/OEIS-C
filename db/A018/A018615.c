/*
 * A018615  Divisors of 730.
 * https://oeis.org/A018615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   Divisors of 730 in increasing order
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t divisors[16];
static idx_t divisor_count;
static bool initialized = false;

static void generate_divisors(const idx_t *primes, const idx_t *exps,
                              size_t prime_cnt, size_t pos,
                              seq_t current, seq_t *out, size_t *out_idx)
{
    if (pos == prime_cnt) {
        out[*out_idx] = current;
        ++(*out_idx);
        return;
    }
    seq_t val = current;
    for (idx_t e = 0; e <= exps[pos]; ++e) {
        generate_divisors(primes, exps, prime_cnt, pos + 1, val, out, out_idx);
        val *= (seq_t)primes[pos];
    }
}

static void build_divisors(void)
{
    /* Factor 730 = 2 × 5 × 73 */
    const idx_t target = (idx_t)730;
    idx_t n = target;
    idx_t primes[8];
    idx_t exps[8];
    size_t prime_cnt = 0;

    /* Find prime factors */
    for (idx_t p = 2; p * p <= n; p += (p == 2 ? 1 : 2)) {
        if (n % p == 0) {
            idx_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            primes[prime_cnt] = p;
            exps[prime_cnt] = exp;
            ++prime_cnt;
        }
    }
    if (n > 1) {
        primes[prime_cnt] = n;
        exps[prime_cnt] = 1;
        ++prime_cnt;
    }

    /* Compute divisor count */
    divisor_count = 1;
    for (size_t i = 0; i < prime_cnt; ++i)
        divisor_count *= (exps[i] + 1);

    /* Generate all divisors */
    size_t idx = 0;
    generate_divisors(primes, exps, prime_cnt, 0, (seq_t)1, divisors, &idx);

    /* Sort ascending */
    for (size_t i = 1; i < divisor_count; ++i) {
        seq_t key = divisors[i];
        size_t j = i;
        while (j > 0 && divisors[j - 1] > key) {
            divisors[j] = divisors[j - 1];
            --j;
        }
        divisors[j] = key;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        build_divisors();

    if (n == 0 || n > divisor_count)
        return 0;

    return divisors[n - 1];
}
