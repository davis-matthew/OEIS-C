/*
 * A048153  a(n) = Sum_{k=1..n} (k^2 mod n).
 * https://oeis.org/A048153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (k^2 mod n)
 * Range:     u8 n <= 25, u16 n <= 364, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A048153.
 * The function runs in O(n) arithmetic operations and
 * uses only the integer types mentioned. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Undefined for n<1, return 0 by convention */

    seq_t sum = 0;
    idx_t nn = n;         /* local copy of n in native integer type */
    for (idx_t k = 1; k < nn; ++k) {
        /* Compute k^2 modulo n using idx_t to avoid intermediate overflow. */
        seq_t mod_val = ( ( (idx_t)k * (idx_t)k ) % nn );
        sum += mod_val;
    }
    return sum;
}
