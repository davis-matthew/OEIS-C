/*
 * A075823  Numbers that are not the last two digits (leading zeros omitted)
 *          of any perfect power.
 * https://oeis.org/A075823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th positive integer whose last two digits
 * (leading zeros omitted) are not equal to any last-two-digit residue
 * of a perfect power (m^k, m ≥ 2, k ≥ 2).  The set of forbidden residues
 * is computed once and reused for all calls. */

#include <stdbool.h>

static bool excluded[100] = {false};

/* Compute all residues modulo 100 that occur as the last two digits of a perfect power
 * with base in [2,99] and exponent ≥ 2.  This set is sufficient for all perfect powers. */
static void init_excluded(void)
{
    static bool done = false;
    if (done)
        return;

    for (int b = 2; b < 100; ++b) {
        unsigned seen[100] = {0};
        unsigned base_mod = (unsigned)b;           /* b < 100 */
        unsigned power = (unsigned)((unsigned long long)base_mod * base_mod % 100ull);
        while (!seen[power]) {
            seen[power] = 1;
            excluded[power] = true;
            power = (unsigned)((unsigned long long)power * base_mod % 100ull);
        }
    }
    done = true;
}

/* Return the n-th element of A075823 (1‑based). */
seq_t a(idx_t n)
{
    init_excluded();

    idx_t count = 0;
    idx_t i = 1;
    for (;;) {
        unsigned r = (unsigned)(i % 100ull);   /* residue modulo 100 */
        if (!excluded[r]) {
            ++count;
            if (count == n)
                return (seq_t)i;
        }
        ++i;
    }
}
