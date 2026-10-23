/*
 * A036193  Log base 2 (n) mod 163.
 * https://oeis.org/A036193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the discrete logarithm base 2 of n modulo 163, * i.e.
 *            find the unique e (0 ≤ e < 162) such that 2^e ≡ n (mod 163). *
 *            For n % 163 == 0 the value is defined as 0
 * Range:     u8 n <= 162, u16 n <= 162, u32 n <= 162, u64 n <= 162,
 *            u128 n <= 162, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static seq_t log_map[163];
static bool log_map_initialized = false;

static void init_discrete_log(void)
{
    seq_t val = 1;          /* 2^0 mod 163 */
    for (idx_t e = 0; e < 162; ++e) {
        size_t idx = (size_t)val;   /* val is always < 163 */
        log_map[idx] = e;
        val = (val * 2) % 163;
    }
}

/* External linkage as required */
seq_t a(idx_t n)
{
    if (!log_map_initialized) {
        init_discrete_log();
        log_map_initialized = true;
    }
    idx_t mod = n % 163;
    if (mod == 0) return 0;          /* defined value for multiples of 163 */
    return log_map[(size_t)mod];
}
