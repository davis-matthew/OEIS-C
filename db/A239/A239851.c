/*
 * A239851  Number of n X 1 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or zero plus the sum of elements above
 *          it or one plus the sum of the elements diagonally to its northwest
 *          or one plus the sum of the elements antidiagonally to its
 *          northeast, modulo 4.
 * https://oeis.org/A239851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 2*a(n-3) for n ≥ 4, with a(1) = 2, a(2) = 2,
 *            a(3) = 4
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 52, u64 n <= 105,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly. */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;

    /* Use a ring buffer of the last three sequence values. */
    seq_t v0 = (seq_t)2; /* a(n‑3) */
    seq_t v1 = (seq_t)2; /* a(n‑2) */
    seq_t v2 = (seq_t)4; /* a(n‑1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = v1 + ((seq_t)2 * v0);  /* a(i) = a(i‑2) + 2*a(i‑3) */
        v0 = v1;            /* shift window: new a(n‑3) */
        v1 = v2;            /* new a(n‑2) */
        v2 = next;          /* new a(n‑1) */
    }

    return v2;
}
