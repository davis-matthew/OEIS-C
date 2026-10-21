/*
 * A067048  a(n) = lcm(n, n+1, n+2, n+3, n+4) / 60.
 * https://oeis.org/A067048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = lcm(n, n+1, n+2, n+3, n+4) / 60
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 163, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute greatest‑common‑divisor for seq_t using the Euclidean algorithm.  */
static inline seq_t _gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  Return 0 for the offset–0 value. */
    if (n == 0)
        return 0;

    seq_t l = 1;                           /* current lcm of processed numbers */

    for (idx_t i = 0; i < 5; ++i)
    {
        seq_t k = (seq_t)(n + i);           /* nth, (n+1), …, (n+4)   */
        seq_t g = _gcd_seq_t(l, k);         /* gcd(l, k)               */
        seq_t div = l / g;                  /* l/g   (integer division) */

        /* Use 128‑bit arithmetic to avoid overflow in the intermediate product. */
        unsigned __int128 prod = (unsigned __int128)div * (unsigned __int128)k;
        l = (seq_t)prod;                    /* cast back to seq_t           */
    }

    /* Final division by 60; use 128‑bit arithmetic to preserve precision. */
    unsigned __int128 final = (unsigned __int128)l / 60;
    return (seq_t)final;
}
