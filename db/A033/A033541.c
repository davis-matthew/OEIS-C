/*
 * A033541  Number of irreducible exceptional curves of first kind on del
 *          Pezzo surface of degree 9-n.
 * https://oeis.org/A033541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (geometric definition): a(n) is the number of -1‑curves on the
 * del Pezzo surface of degree 9‑n.
 * For n ≤ 4 this equals the triangular number n(n+1)/2.
 * For n = 5,6,7,8 the values are 16, 27, 56, 240 respectively,
 * and for n ≥ 9 there are no such surfaces, so a(n)=0. */

seq_t a(idx_t n)
{
    /* Difficulty: for n ≤ 4 the count is triangular; for the remaining
     * admissible values it is one of a few well‑known constants.
     * The function therefore uses no external tables and
     * satisfies the time limit for n up to 10 000 (trivial). */

    if (n > 8) return (seq_t)0;          /* no del Pezzo surfaces */

    if (n <= 4) {                      /* triangular numbers */
        return (seq_t)(n * ((seq_t)n + 1) / 2);
    }

    /* Remaining small n – explicit exact values. */
    switch ((idx_t)n) {
        case 5: return (seq_t)16;
        case 6: return (seq_t)27;
        case 7: return (seq_t)56;
        case 8: return (seq_t)240;
    }

    /* unreachable */
    return (seq_t)0;
}
