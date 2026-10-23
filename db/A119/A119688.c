/*
 * A119688  a(n) = n!! mod (n+1).
 * https://oeis.org/A119688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n!! (double factorial) mod (n+1)
 * Range:     u8 n <= 260, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = product of all integers of the same parity as n, from n down to 1 or 2,
 *        taken modulo (n+1).  Here n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not used, offset starts at 1 */

    seq_t mod    = (seq_t)n + 1;          /* divisor (n+1) */
    seq_t result = 1;                    /* accumulator */

    idx_t i = n;
    while (i > 0) {
        /* multiply by i and reduce modulo mod, using 128‑bit arithmetic
           when available to avoid overflow on 64‑bit builds. */
#ifdef __SIZEOF_INT128__
        unsigned __int128 t = (unsigned __int128)result *
                               (unsigned __int128)i;
        result = (seq_t)(t % mod);
#else
        result = (seq_t)((result * i) % mod);
#endif
        /* stop after processing the smallest element (1 or 2) */
        if (i <= 2) break;
        i -= 2;
    }
    return result;
}
