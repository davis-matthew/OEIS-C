/*
 * A018478  Divisors of 480.
 * https://oeis.org/A018478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 480, sorted ascending */
/* The function returns the n‑th divisor of 480 (a(1)=1, a(24)=480). */

#include <stddef.h>
#include <stdbool.h>

static bool initialized = false;
static seq_t cache[25];
static idx_t cache_size = 0;

/* Recursive generator of all divisors. */
static void gen(idx_t idx, seq_t cur,
                const idx_t primes[], const idx_t exps[],
                idx_t num, seq_t *out, idx_t *cnt)
{
    if (idx == num) {
        out[(*cnt)++] = cur;
        return;
    }
    seq_t val = 1;
    for (idx_t e = 0; e <= exps[idx]; ++e) {
        gen(idx + 1, cur * val, primes, exps, num, out, cnt);
        val *= primes[idx];
    }
}

/* Build the divisor table once. */
static void build_divisors(void)
{
    static const idx_t primes[3]   = {2, 3, 5};
    static const idx_t exps[3]     = {5, 1, 1};
    static seq_t divs[25];
    idx_t count = 0;

    gen(0, 1, primes, exps, 3, divs, &count);

    /* Sort ascending – sequence is required in ascending order. */
    for (idx_t i = 0; i < count; ++i) {
        for (idx_t j = i + 1; j < count; ++j) {
            if (divs[j] < divs[i]) {
                seq_t tmp = divs[i];
                divs[i] = divs[j];
                divs[j] = tmp;
            }
        }
    }

    for (idx_t i = 0; i < count; ++i) cache[i] = divs[i];
    cache_size = count;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if (!initialized) {
        build_divisors();
        initialized = true;
    }

    if (n <= cache_size) return cache[n - 1];
    return 0;   /* Undefined for n > 24 */
}
