/*
 * A213506  Number of nonisomorphic 2-generator p-groups of class at most 2
 *          and order p^n.
 * https://oeis.org/A213506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( ((n^4+26*n^3+234*n^2+(909-64*(n mod 3))*n+1701 * +
 *            27*(n+1)*(-1)^n) / 1728 )
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 251, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A213506: number of nonisomorphic 2‑generator p‑groups of
 * class at most 2 and order p^n, offset 0. */
seq_t a(idx_t n)
{
    /* convert index to seq_t */
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;            /* n^2 */
    seq_t n3 = n2 * n_seq;               /* n^3 */
    seq_t n4 = n3 * n_seq;               /* n^4 */

    /* (n^4 + 26*n^3 + 234*n^2) */
    seq_t part = n4
               + (seq_t)26 * n3
               + (seq_t)234 * n2;

    /* (909-64*(n mod 3))*n */
    idx_t rem3 = n % (idx_t)3;
    seq_t coeff = (seq_t)909 - (seq_t)64 * (seq_t)rem3;
    part += coeff * n_seq;

    /* constant term 1701 */
    part += (seq_t)1701;

    /* add or subtract 27*(n+1)*(-1)^n */
    seq_t add = (seq_t)27 * (n_seq + (seq_t)1);
    if (n & (idx_t)1)          /* n odd: subtract */
        part -= add;
    else                      /* n even: add */
        part += add;

    /* divide by 1728 (floor division) */
    return part / (seq_t)1728;
}
