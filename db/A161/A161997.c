/*
 * A161997  Number of diagrams for dimension n in table 8.1 of the paper by
 *          Felikson & Tumarkin.
 * https://oeis.org/A161997
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 20, 6, 2, 1, 1 for n = 4,5,6,7,8 respectively and 0
 *            otherwise
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from the list of known essential hyperbolic Coxeter polytopes in
   Felikson & Tumarkin’s Table 8.1; no such diagrams exist for dimensions > 8. */
seq_t a(idx_t n) {
    /* n is of type idx_t (uint64_t). Cast to use in comparisons. */
    if (n == 4) return (seq_t)20;
    if (n == 5) return (seq_t)6;
    if (n == 6) return (seq_t)2;
    if (n == 7) return (seq_t)1;
    if (n == 8) return (seq_t)1;
    return (seq_t)0;  /* for n > 8 (and also for any n < 4, though not required) */
}
