/*
 * A084557  a(0)=0, after which each n occurs A084556(n) times.
 * https://oeis.org/A084557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ k ≥ 1 | Σ_{i=1}^k L(i) ≥ n }, where L(i) is the
 *            length of the i‑th permutation in lexicographic order of all
 *            permutations. L(i) equals the smallest ℓ with i ≤ Σ_{m=1}^ℓ m!.
 *            Equivalently, let Tℓ = Σ_{m=1}^ℓ m·m! (cumulative number of
 *            terms up to length ℓ) Sℓ = Σ_{m=1}^ℓ m! (cumulative...
 * Range:     u8 n <= 1331, u16 n <= 9675, u32 n <= 9675, u64 n <= 9675,
 *            u128 n <= 9675, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    unsigned __int128 fact = 1;          /* ℓ!    */
    idx_t          perms_before = 0;     /* S_{ℓ-1} */
    unsigned __int128 terms_before = 0;  /* T_{ℓ-1} */

    for (idx_t l = 1; ; ++l)
    {
        unsigned __int128 l_mul_fact = (unsigned __int128)l * fact;
        unsigned __int128 new_terms_before = terms_before + l_mul_fact;

        /* If n lies in this block of permutations of length l,
           compute the corresponding permutation index. */
        if ((unsigned __int128)n <= new_terms_before)
        {
            idx_t offset = (n - terms_before - 1) / l;
            return (seq_t)(perms_before + offset + 1);
        }

        /* Otherwise skip this whole block and move to the next length. */
        perms_before += (idx_t)fact;
        terms_before = new_terms_before;
        fact *= (l + 1);   /* next factorial */
    }
}
