/*
 * A036203  Log base 3 (n) mod 223.
 * https://oeis.org/A036203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete logarithm of n modulo 223 to the * base 3
 *            (smallest e ≥ 0 with 3^e ≡ n (mod 223)); if n ≡ 0 (mod 223) the
 *            * result is 0
 * Range:     u8 n <= 222, u16 n <= 222, u32 n <= 222, u64 n <= 222,
 *            u128 n <= 222, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t log3_table[223] = {0};
static bool log3_initialized = false;

static void init_log3_table(void)
{
    if (log3_initialized)
        return;

    idx_t pow = 1;       /* 3^0 mod 223 */
    for (int e = 0; e < 222; ++e) {
        log3_table[pow] = (seq_t)e;   /* exponent e for residue pow */
        pow = (pow * 3u) % 223u;      /* next power */
    }

    log3_initialized = true;
}

seq_t a(idx_t n)
{
    init_log3_table();

    idx_t rem = n % 223u;
    return log3_table[(unsigned int)rem];
}
