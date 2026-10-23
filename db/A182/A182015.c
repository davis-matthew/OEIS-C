/*
 * A182015  Diagonal sums of triangle A182013.
 * https://oeis.org/A182015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=0..n} (n-i+1)*M(i) - sum_{i=0..floor(n/2)}
 *            (n-2i)*M(i) where M(i) are Motzkin numbers, M(0)=1, M(1)=1,
 *            M(n)=((2n+1)*M(n-1)+3(n-1)*M(n-2))/(n+2) for n>=2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t       M_len  = 0;        /* one past the last computed index   */
static idx_t       M_cap  = 0;        /* allocated capacity for the arrays   */
static seq_t      *M      = NULL;     /* Motzkin numbers                     */
static seq_t      *S1     = NULL;     /* prefix sums of M: S1[i] = Σ_{k=0..i} M(k) */
static seq_t      *S2     = NULL;     /* prefix sums of k*M(k)               */

static void ensure_cap(idx_t needed)
{
    if (needed <= M_cap) return;
    size_t new_cap = M_cap ? M_cap << 1 : 1;
    while (new_cap < (size_t)needed) new_cap <<= 1;
    M_cap = (idx_t)new_cap;
    M   = (seq_t *)realloc(M,   M_cap * sizeof(seq_t));
    S1  = (seq_t *)realloc(S1,  M_cap * sizeof(seq_t));
    S2  = (seq_t *)realloc(S2,  M_cap * sizeof(seq_t));
}

static void extend_to(idx_t n)
{
    if (n + 1 <= M_len) return;                /* already computed          */
    ensure_cap(n + 1);

    for (idx_t i = M_len; i <= n; ++i)
    {
        if (i == 0)
        {
            M[0] = (seq_t)1;
            S1[0] = (seq_t)1;
            S2[0] = (seq_t)0;
        }
        else if (i == 1)
        {
            M[1] = (seq_t)1;
            S1[1] = S1[0] + M[1];
            S2[1] = S2[0] + ((seq_t)1) * M[1];
        }
        else
        {
            seq_t term1 = ((seq_t)(2 * i + 1)) * M[i - 1];
            seq_t term2 = ((seq_t)(3 * (i - 1))) * M[i - 2];
            seq_t num   = term1 + term2;
            M[i]        = num / (idx_t)(i + 2);
            S1[i]       = S1[i - 1] + M[i];
            S2[i]       = S2[i - 1] + ((seq_t)i) * M[i];
        }
    }
    M_len = n + 1;
}

seq_t a(idx_t n)
{
    extend_to(n);

    seq_t nn         = (seq_t)n;
    seq_t first_part = (nn + 1) * S1[n] - S2[n];

    idx_t half       = n / 2;
    seq_t second_part = nn * S1[half] - (seq_t)2 * S2[half];

    return first_part - second_part;
}
