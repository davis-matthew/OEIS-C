/*
 * A239140  Number of strict partitions of n having standard deviation σ < 1.
 * https://oeis.org/A239140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + [n odd && n≥3] + [n mod 3 == 0 && n≥6]
 * Range:     u8 n <= 10005, u16 n <= 10005, u32 n <= 10005, u64 n <= 10005,
 *            u128 n <= 10005, big any n
 * Validated: standard, pass, 102 terms through n = 9953 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* a(1)=1, a(2)=1, a(n) (n≥3) constructed as described above */
    seq_t result = 1;                /* the one‑part partition {n} */

    if (n >= 3 && (n & 1ULL))
        result += 1;                /* two consecutive parts { (n−1)/2, (n+1)/2 } */

    if (n >= 6 && (n % 3ULL) == 0)
        result += 1;                /* three consecutive parts { n/3−1, n/3, n/3+1 } */

    return result;
}
