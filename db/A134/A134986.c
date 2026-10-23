/*
 * A134986  a(n) = smallest integer m not equal to n such that n =
 *          (floor(n^2/m) + m)/2.
 * https://oeis.org/A134986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer m ≠ n such that n = (⌊n² / m⌋ + m)/2. So
 *            floor(n² / m) = 2·n – m. This routine scans the candidates 1…2·n
 *            and returns the first one that satisfies the equation
 * Range:     u8 n <= 15, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the 64‑bit index to the result type for work. */
    seq_t n_seq = (seq_t)n;

    /* 2·n (used twice). */
    seq_t two_n = (seq_t)2 * n_seq;

    /* n² – compute in a type that can hold the intermediate. */
    seq_t n2;
#ifdef __cplusplus
    /* In C++ the multiplication is performed by seq_t’s operator*. */
    n2 = n_seq * n_seq;
#else
    /* In C use built‑in unsigned __int128 to avoid overflow. */
    unsigned __int128 tmp = (unsigned __int128)n_seq * n_seq;
    n2 = (seq_t)tmp;
#endif

    /* upper bound for m: for all n ≥ 1 the equality cannot hold for
       m > 2·n, and m = 2 is the solution for n = 1. */
    idx_t limit = (idx_t)(two_n);          // safe: two_n fits in idx_t
    for (idx_t m = (idx_t)1; m <= limit; ++m) {
        if (m == n) continue;              /* m must differ from n */

        /* floor(n² / m). */
        seq_t f;
#ifdef __cplusplus
        f = n2 / (seq_t)m;
#else
        f = (seq_t)((unsigned __int128)n2 / m);
#endif

        /* Right‑hand side 2·n – m. */
        seq_t rhs = two_n - (seq_t)m;

        if (f == rhs)                      /* found the smallest m */
            return (seq_t)m;
    }

    /* The definition guarantees a solution for all n ≥ 1,
       so this line is never reached. */
    return (seq_t)1;
}
