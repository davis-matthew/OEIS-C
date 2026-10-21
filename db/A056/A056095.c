/*
 * A056095  Numbers k such that k^12 == 1 (mod 13^4).
 * https://oeis.org/A056095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 30, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: precompute the 12 residues r modulo 28561 (=13^4)
   satisfying r^12 ≡ 1 (mod 28561). The set of solutions is a subgroup
   of order 12, so the sequence repeats these residues indefinitely.
   a(n) = residues[(n-1) mod 12] + 28561 * floor((n-1)/12). */
seq_t a(idx_t n)
{
    /* static cache of residues */
    static unsigned long long residues[12];
    static int init = 0;
    static const unsigned long long M = 28561ULL; /* 13^4 */
    if (!init)
    {
        unsigned long long cnt = 0;
        for (unsigned long long k = 1; k < M; ++k)
        {
            /* compute k^12 mod M */
            unsigned long long t = 1;
            for (int i = 0; i < 12; ++i)
            {
                t = (t * k) % M;
            }
            if (t == 1ULL)
            {
                residues[cnt++] = k;
            }
        }
        /* there should be exactly 12 residues */
        /* simple insertion sort (cnt is small) */
        for (unsigned long long i = 0; i < cnt; ++i)
        {
            for (unsigned long long j = i + 1; j < cnt; ++j)
            {
                if (residues[j] < residues[i])
                {
                    unsigned long long tmp = residues[i];
                    residues[i] = residues[j];
                    residues[j] = tmp;
                }
            }
        }
        init = 1;
    }

    /* compute index within the periodic block */
    unsigned long long offset = n - 1;
    unsigned long long q = offset / 12;   /* number of full periods */
    unsigned long long r = offset % 12;   /* index inside the period */

    /* result = residue + M * q, using seq_t arithmetic to avoid overflow */
    seq_t result = (seq_t)residues[r] + (seq_t)M * (seq_t)q;
    return result;
}
