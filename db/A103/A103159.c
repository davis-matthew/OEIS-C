/*
 * A103159  a(n) = GCD(reverse(n), reversed(n+1)).
 * https://oeis.org/A103159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = gcd(reverse(n), reverse(n+1))
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _gcd_seq_local, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rev.h"
#include "../utils/_gcd_seq_local.h"

seq_t a(idx_t n)
{
    seq_t rev_n   = rev((seq_t)n);
    seq_t rev_np1 = rev((seq_t)(n + 1));
    return _gcd_seq_local(rev_n, rev_np1);
}
