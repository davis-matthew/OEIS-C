/*
 * A186331  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f and g are the pentagonal numbers and
 *          the hexagonal numbers. Complement of A186330.
 * https://oeis.org/A186331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of hexagonal numbers ≤ H_n + number of pentagonal
 *            numbers < H_n where H_n = n(2n–1) is the nth hexagonal number
 * Range:     u8 n <= 2, u16 n <= 37, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = 1;
    /* Find an upper bound for sqrt(x). */
    while ((__int128)high * high <= x) {
        high <<= 1;
    }
    /* Binary search for the integer square root. */
    while (low + 1 < high) {
        seq_t mid = low + ((high - low) >> 1);
        if ((__int128)mid * mid <= x) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Compute the nth hexagonal number H_n = n(2n−1). */
    seq_t nn = (seq_t)n;
    seq_t x  = nn * ((seq_t)2 * nn - 1);

    /* Count hexagonal numbers ≤ x.  H_k = k(2k−1). */
    seq_t v1  = (seq_t)1 + (seq_t)8 * x;
    seq_t s1  = isqrt_seq(v1);
    seq_t h_le = (s1 + 1) / 4;          /* floor((1+sqrt(v1))/4) */

    /* Count pentagonal numbers < x.  P_k = k(3k−1)/2. */
    seq_t v2  = (seq_t)1 + (seq_t)24 * x;
    seq_t s2  = isqrt_seq(v2);
    seq_t pk  = (s2 + 1) / 6;           /* initial guess for floor((1+sqrt(v2))/6) */

    /* Adjust if P_k is not < x.  Decrement until it fits. */
    while (pk > 0) {
        seq_t pent = pk * ((seq_t)3 * pk - 1) / 2;
        if (pent < x) break;
        pk--;
    }

    /* The required rank is h_le + pk (pk is the count of pentagonal < x). */
    return h_le + pk;
}
