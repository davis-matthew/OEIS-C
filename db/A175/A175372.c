/*
 * A175372  Number of integer pairs (x,y) satisfying x^4 + y^4 = n.
 * https://oeis.org/A175372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer pairs (x,y) with x⁴ + y⁴ = n
 * Range:     u8 n <= 15, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count integer solutions to x⁴ + y⁴ = n.  This is a direct, O(n¹/⁴)
   algorithm: for each x with 0 ≤ x⁴ ≤ n, compute the remaining
   fourth‑power and add the appropriate multiplicity.  */
static inline seq_t mul128(seq_t a, seq_t b)
{
    return (seq_t)((__int128)a * b);
}

static inline seq_t square(seq_t a)
{
    return mul128(a, a);
}

static inline seq_t floor_fourth_root(seq_t n)
{
    seq_t low = 0;
    seq_t high = 1;
    /* Increase high until high⁴ > n */
    while (1)
    {
        seq_t h2 = square(high);
        seq_t h4 = mul128(h2, h2);
        if (h4 > n) break;
        high <<= 1;
        if (high == 0) break;  /* safeguard against overflow */
    }
    /* Binary search for the largest m with m⁴ ≤ n */
    while (low < high)
    {
        seq_t mid = (low + high + 1) >> 1;
        seq_t m2 = square(mid);
        seq_t m4 = mul128(m2, m2);
        if (m4 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t count = 0;
    seq_t limit = floor_fourth_root(nn);
    for (seq_t x = 0; x <= limit; ++x)
    {
        seq_t x4 = square(square(x));
        if (x4 > nn) break;
        seq_t rem = nn - x4;
        seq_t y = floor_fourth_root(rem);
        if (square(square(y)) != rem) continue;   /* y⁴ ≠ rem */
        if (y < x) continue;                     /* enforce x ≤ y to avoid double count */
        if (x == 0 && y == 0)
            count += 1;                          /* (0,0) only once */
        else if (x == 0 || y == 0)
            count += 4;                          /* one zero: (±a,0),(0,±a) */
        else if (x == y)
            count += 4;                          /* a⁴ + a⁴: (±a,±a) */
        else
            count += 8;                          /* a⁴ + b⁴: (±a,±b),(±b,±a) */
    }
    return count;
}
