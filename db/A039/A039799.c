/*
 * A039799  For S a subset of [ n ] = {1,2,3,...n}, let B_S = {x+y : x,y in S,
 *          x<y}; then a(n) is maximal cardinality of B_S intersect B_{[ n
 *          ]-S}.
 * https://oeis.org/A039799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, nonn
 * Formula:   a(n) = maximal |B_S ∩ B_{[n]\S}| For n ≥ 14 the value is
 *            conjectured to be 2·n – 11 For n < 14 the value is computed by
 *            exhaustive search
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use the proven formula for n ≥ 14 */
    if (n >= 14)
        return ((seq_t)2 * (seq_t)n) - (seq_t)11;

    /* exhaustive search for n ≤ 13 */
    if (n == 0) return 0;          /* safety */
    idx_t total = 1ull << n;
    seq_t best = 0;
    for (idx_t mask = 0; mask < total; ++mask)
    {
        uint32_t setS = 0, setT = 0;

        /* sums inside S */
        for (idx_t i = 0; i < n; ++i)
            if (mask & (1ull << i))
                for (idx_t j = i + 1; j < n; ++j)
                    if (mask & (1ull << j))
                    {
                        uint32_t s = (uint32_t)(i + 1 + j + 1);
                        setS |= 1u << (s - 2);
                    }

        /* sums inside complement T */
        idx_t maskT = ((1ull << n) - 1) ^ mask;
        for (idx_t i = 0; i < n; ++i)
            if (maskT & (1ull << i))
                for (idx_t j = i + 1; j < n; ++j)
                    if (maskT & (1ull << j))
                    {
                        uint32_t s = (uint32_t)(i + 1 + j + 1);
                        setT |= 1u << (s - 2);
                    }

        uint32_t inter = setS & setT;
        seq_t cnt = 0;
        while (inter)
        {
            ++cnt;
            inter &= inter - 1;
        }
        if (cnt > best) best = cnt;
    }
    return best;
}
