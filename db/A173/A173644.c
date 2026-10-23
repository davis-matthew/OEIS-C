/*
 * A173644  a(n) = smallest positive integer m such that n^2+7m is a square.
 * https://oeis.org/A173644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ( (k^2 - n^2) / 7 ), where k is the smallest integer > n such that
 *                k^2 ≡ n^2 (mod 7).  Equivalently, k ≡ n (mod 7) or k ≡ ±n (mod 7)
 *                depending on the residue of n^2 modulo 7.
 */
#include <stdint.h> /* for uint8_t etc., if needed */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type. */
    seq_t n_seq = (seq_t)n;

    /* The residue of n^2 modulo 7.  Since 7 fits in `int`, we can use `int`
       for these modular arithmetic operations. */
    int n_mod   = (int)(n % 7);
    int n_sq_mod = (int)((n_mod * n_mod) % 7);

    /* Find the smallest positive step `d` such that (n + d)^2 ≡ n^2 (mod 7). */
    int d = 1;
    while (1) {
        int k_mod = (int)((n_mod + d) % 7);
        if ((k_mod * k_mod) % 7 == n_sq_mod)
            break;
        ++d;
        /* The loop will terminate in at most 6 iterations because the
           modulo 7 condition can only fail at most 6 times before it
           succeeds for some residue. */
    }

    /* Compute the corresponding k. */
    seq_t k_seq = n_seq + (seq_t)d;

    /* Compute the minimal m = (k^2 - n^2) / 7. */
    seq_t diff = k_seq * k_seq - n_seq * n_seq;
    return diff / 7;
}
