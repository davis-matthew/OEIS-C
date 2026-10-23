/*
 * A036209  Log base 6 (n) mod 251.
 * https://oeis.org/A036209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete_log_6(n) mod 251 i.e. the exponent e, 0 ≤ e <
 *            250, such that 6^e ≡ n (mod 251)
 * Range:     u8 n <= 250, u16 n <= 250, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t logtable[251];
    static int ready = 0;

    if (!ready) {
        /* build log table: logtable[r] = e where 6^e ≡ r (mod 251) for 1 ≤ r ≤ 250 */
        seq_t cur = 1;
        for (int i = 0; i < 250; ++i) {
            logtable[(size_t)cur] = (seq_t)i;
            cur = (cur * 6) % 251;
        }
        /* residue 0 has no discrete logarithm – leave logtable[0] = 0 */
        ready = 1;
    }

    idx_t mod = n % 251;
    if (mod == 0) {
        return (seq_t)0;          /* undefined log; return 0 as default */
    }
    return logtable[(size_t)mod];
}
