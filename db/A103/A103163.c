/*
 * A103163  a(n) = gcd(reverse(prime(n)), reverse(prime(n+1))).
 * https://oeis.org/A103163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = gcd(reverse(prime(n)), reverse(prime(n+1)))
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_seq, get_prime, rev, rev_seq_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/rev.h"
#include "../utils/gcd_seq.h"
#include "../utils/rev_seq_local.h"

seq_t a(idx_t n) {
    /* offset is 1 – a(1) is the first term */
    seq_t p1 = get_prime(n);       /* prime(n)   */
    seq_t p2 = get_prime(n + 1);   /* prime(n+1) */

    seq_t r1 = rev_seq_local(p1);  /* reverse of prime(n)   */
    seq_t r2 = rev_seq_local(p2);  /* reverse of prime(n+1) */

    return gcd_seq(r1, r2);        /* gcd of the two reverses */
}
