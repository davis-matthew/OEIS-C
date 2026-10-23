/*
 * A185353  a(n) = (1^1 + 2^2 . . . + n^n) mod 10.
 * https://oeis.org/A185353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1^1 + 2^2 + ... + n^n) mod 10
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodicity: the sequence a(n) has period 100. The array
   period[0..99] stores a(1) ... a(100). The function uses a
   static lookup table computed on first use. */

#include <stddef.h>

static seq_t period[100] = {0};
static int period_initialized = 0;

/* Compute b^e mod 10 where b and e are small integers (e up to 10000). */
static seq_t mod_pow10(idx_t b, idx_t e)
{
    seq_t base = (seq_t)(b % 10);
    seq_t result = 1 % 10;
    while (e > 0) {
        if (e & 1) {
            result = (result * base) % 10;
        }
        base = (base * base) % 10;
        e >>= 1;
    }
    return result;
}

/* Fill the period array once. */
static void init_period(void)
{
    if (period_initialized) return;
    seq_t sum = 0;
    for (idx_t k = 1; k <= 100; ++k) {
        sum = (sum + mod_pow10(k, k)) % 10;
        period[(size_t)k - 1] = sum;
    }
    period_initialized = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; 0 is undefined */
    init_period();
    /* n-1 because period[0] = a(1) */
    size_t idx = (size_t)((n - 1) % 100);
    return period[idx];
}
