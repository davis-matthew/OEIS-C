/*
 * A067938  Let rep(n) be the n-th repunit number, sequence gives values of n
 *          such that rep(n)==rep(5) (mod n).
 * https://oeis.org/A067938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th positive integer m for which rep(m) == rep(5)
 *            (mod m)
 * Range:     u8 n <= 10, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here rep(k) = (10^k - 1)/9 is the k‑th repunit number.  */

/* We test the condition as
 *   10^m ≡ 10^5 (mod 9·m)
 * because
 *   rep(m) ≡ rep(5) (mod m)
 *   ⇔ (10^m -1)/9 ≡ 11111 (mod m)
 *   ⇔ 10^m -1 ≡ 99999 (mod 9m)
 *   ⇔ 10^m ≡ 100000 (= 10^5) (mod 9m).  */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    idx_t target = n;          /* the required index (1‑based)  */
    idx_t count  = 0;          /* number of found terms so far */
    idx_t m      = 0;          /* current candidate integer   */

    /* iterate m = 1, 2, … until we have found target terms */
    while (count < target)
    {
        ++m;
        /* modulus for the congruence: 9·m                     */
        idx_t mod_val = 9 * m;

        /* compute 10^m  (mod 9·m) using the provided modular
           exponentiation routine (works for any seq_t size).   */
        seq_t pow10m = mod_pow((seq_t)10, m, (seq_t)mod_val);

        /* 10^5 mod 9·m, i.e. 100000 % (9·m) */
        seq_t rhs = (seq_t)(100000 % mod_val);

        /* check the congruence */
        if (pow10m == rhs)
            ++count;
    }

    /* m now is the n-th element of the sequence */
    return (seq_t)m;
}
