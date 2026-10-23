/*
 * A185374  Number of holes in n-th lower case letter of English alphabet (in
 *          printed form using standard Times New Roman font).
 * https://oeis.org/A185374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = holes[ ((n-1) mod 26) ]
 * where holes contains the number of holes in each lower‑case
 * English letter printed in Times New Roman.
 */
seq_t a(idx_t n)
{
    /* Return zero for n == 0 (out of range, not expected). */
    if (n == 0) return (seq_t)0;

    /* Number of holes for 'a'..'z'. */
    static const seq_t holes[26] = {
        1, /* a */
        1, /* b */
        0, /* c */
        1, /* d */
        1, /* e */
        0, /* f */
        2, /* g */
        0, /* h */
        0, /* i */
        0, /* j */
        0, /* k */
        0, /* l */
        0, /* m */
        0, /* n */
        1, /* o */
        1, /* p */
        1, /* q */
        0, /* r */
        0, /* s */
        0, /* t */
        0, /* u */
        0, /* v */
        0, /* w */
        0, /* x */
        0, /* y */
        0  /* z */
    };

    /* Compute index 0..25 using modulo so the sequence cycles. */
    idx_t idx = (n - 1) % 26;
    return holes[idx];
}
