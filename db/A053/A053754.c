/*
 * A053754  If k is in the sequence then 2*k and 2*k+1 are not (and 0 is in
 *          the sequence); when written in binary k has an even number of bits
 *          (0 has 0 digits).
 * https://oeis.org/A053754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = k where k is the (n-1)-th non‑negative integer whose
 *            binary length is even (0 has length 0)
 * Range:     u8 n <= 171, u16 n <= 9767, u32 n <= 9767, u64 n <= 9767,
 *            u128 n <= 9767, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1)                           /* a(1) = 0 */
        return (seq_t)0;

    idx_t r = n - 2;                      /* index inside the even‑length blocks (0‑based) */
    idx_t t = 1;                          /* t = 1,2,3,…  → length L = 2*t bits */

    while (1) {
        /* block_size = 2^(L-1) = 2^(2*t-1) */
        idx_t exp = 2 * t - 1;
        idx_t block_size = 1ULL << exp;  /* works for exp ≤ 63 (our n is ≤ ~10000) */

        if (r < block_size)              /* the wanted number lies in this block */
            return ((seq_t)block_size) + (seq_t)r;

        r -= block_size;                 /* skip this block */
        ++t;
    }
}
