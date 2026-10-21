/*
 * A051846  Digits 1..n in strict descending order n..1 interpreted in base
 *          n+1.
 * https://oeis.org/A051846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = Sum_{i=1..n} i * (n+1)^(i-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for arbitrary sequence type seq_t and index type idx_t.
 * For 64‑bit seq_t a safe 128‑bit intermediate is used to avoid
 * overflow while the result still fits in seq_t.
 * For arbitrary‑precision seq_t the natural multiplication operator
 * is used, which can hold the large intermediate values. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* Offset starts at 1, guard against 0 */

    seq_t base = (seq_t)(n + 1);
    seq_t sum   = 0;                     /* Accumulated result */
    seq_t power = 1;                     /* base^(i-1) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t term;
        if (sizeof(seq_t) <= 8) {        /* 64‑bit or smaller: use 128‑bit mul */
            unsigned __int128 prod =
                (unsigned __int128)power * (unsigned __int128)i;
            term = (seq_t)prod;
        } else {                         /* Arbitrary‑precision: normal mul */
            term = (seq_t)i * power;
        }
        sum += term;

        if (i < n) {                    /* Update power for next iteration */
            if (sizeof(seq_t) <= 8) {
                unsigned __int128 prod =
                    (unsigned __int128)power * (unsigned __int128)base;
                power = (seq_t)prod;
            } else {
                power *= base;
            }
        }
    }
    return sum;
}
