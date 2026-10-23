/*
 * A171437  Square array read by antidiagonals: T(m,n) is the Wiener index of
 *          the prism graph C(m) x P(n) (m>=3, n>=1). The Wiener index of a
 *          connected graph is the sum of distances between all unordered
 *          pairs of vertices in the graph.
 * https://oeis.org/A171437
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   * Let (m,k) be the parameters of the prism graph C(m) x P(k). *
 *            The sequence is enumerated by antidiagonals over m≥3, k≥1. * For
 *            a given index n≥3, compute (m,k) as follows: * i = n-3; * Find
 *            the smallest d with (d-3)(d-2)/2 > i. * Let s = i -
 *            (d-3)(d-2)/2. * m = d-1-s , k = d-m . * * The Wiener...
 * Range:     u8 n <= 4, u16 n <= 51, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 3) return 0;           /* undefined for n < 3 */

    /* map n to coordinates (m,k) via antidiagonals */
    idx_t i = n - 3;               /* zero‑based index in the enumeration */
    idx_t d = 4;                   /* current diagonal sum m + k */
    idx_t cumulative = 0;          /* sum of lengths of previous diagonals */

    while (cumulative + (d - 3) <= i) {
        cumulative += (d - 3);
        d++;
    }

    idx_t s = i - cumulative;      /* offset inside the current diagonal */
    idx_t m = d - 1 - s;
    idx_t k = d - m;

    /* compute the Wiener index using the formula */
    seq_t m_s = (seq_t)m;
    seq_t k_s = (seq_t)k;
    seq_t result;

    if ((m & 1U) != 0) {     /* m is odd */
        seq_t m2 = m_s * m_s;
        seq_t k2 = k_s * k_s;
        seq_t part1 = 4 * m_s * (k2 - 1);
        seq_t part2 = 3 * k_s * (m2 - 1);
        seq_t num = m_s * k_s * (part1 + part2);
        result = num / 24;
    } else {                 /* m is even */
        seq_t m2 = m_s * m_s;
        seq_t k2 = k_s * k_s;
        seq_t part1 = 4 * (k2 - 1);
        seq_t part2 = 3 * m_s * k_s;
        seq_t num = m2 * k_s * (part1 + part2);
        result = num / 24;
    }

    return result;
}
