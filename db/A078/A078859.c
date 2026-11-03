/*
 * A078859  Least positive residues (mod 210) representing those residue
 *          classes which can be the lesser of twin prime pairs (A001359).
 * https://oeis.org/A078859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: full, pass, 17 terms through n = 17 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Definition: a(n) is the nth element of the sorted set of residues r modulo 210
 * that can be the smaller member of a twin‑prime pair.  
 * The set S consists of r = 3 or r = 5 (the special pairs (3,5) and (5,7))
 * and all r such that both r and r+2 are coprime with 210.
 * Hence S = { r: r=3 or r=5 or (gcd(r,210)=1 and gcd(r+2,210)=1) }.
 * The set has 17 elements: 3,5,11,17,29,41,59,71,101,107,137,149,167,179,191,197,209.
 */

#include <stddef.h>
#include <stdbool.h>

static void init_residues(seq_t *residues, size_t *count)
{
    size_t idx = 0;
    for (int r = 0; r < 210; ++r)
    {
        if (r == 3 || r == 5)
        {
            residues[idx++] = (seq_t)r;
            continue;
        }
        /* r must be odd and not divisible by 3,5,7 */
        if ((r & 1) == 0)                      continue;
        if (r % 3 == 0 || r % 5 == 0 || r % 7 == 0) continue;

        int rp = r + 2;
        /* rp must also be coprime to 210 */
        if ((rp & 1) == 0)                      continue;
        if (rp % 3 == 0 || rp % 5 == 0 || rp % 7 == 0) continue;

        residues[idx++] = (seq_t)r;
    }
    *count = idx;   /* should be 17 */
}

/* main sequence routine */
seq_t a(idx_t n)
{
    /* static storage for the residue set */
    static seq_t residues[17];
    static bool initialized = false;
    static size_t residue_count = 0;

    if (!initialized)
    {
        init_residues(residues, &residue_count);
        initialized = true;
    }

    if (n == 0 || n > residue_count)
        return 0;   /* values outside the defined sequence are undefined */

    return residues[(size_t)(n - 1)];
}
