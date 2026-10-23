/*
 * A036748  Number of strings of n distinct digits from 0-9 that are the last
 *          n digits of a square in base 6.
 * https://oeis.org/A036748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A036748: number of strings of n distinct base‑6 digits
   that are the last n digits of a square.
   For n>6 no such strings exist, so a(n)=0.  The function counts
   by:
     1. enumerating all residues modulo 6^n that are perfect squares,
     2. generating all n‑digit permutations of the digits 0…5,
     3. counting those permutations that lie in the set of square
        residues.  */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* pre‑allocate cache for n = 0…6 */
static seq_t cache[7];
static bool computed[7];

/* power of 6 up to 6, pre‑computed as seq_t */
static seq_t pow6[7] = {0};

/* visited array size 6^6 = 46 656 */
static unsigned char visited[46656];

/* Forward declaration of recursive generator */
static void gen_permutations(idx_t n, idx_t pos,
                             seq_t val, unsigned int used,
                             seq_t *cnt);

/* Helper to initialise pow6 once */
static void init_pow6(void)
{
    if (pow6[0])
        return;          /* already initialised */
    pow6[0] = 1;
    for (idx_t i = 1; i < 7; ++i)
        pow6[i] = pow6[i-1] * (seq_t)6;
}

seq_t a(idx_t n)
{
    if (n == 0 || n > 6)
        return 0;

    if (!computed[(size_t)n])
    {
        /* 1. initialise powers of 6 */
        init_pow6();

        /* 2. compute 6^n as seq_t and as idx_t index */
        seq_t mod_seq = 1;
        idx_t mod_idx = 1;
        for (idx_t i = 0; i < n; ++i)
        {
            mod_seq *= (seq_t)6;
            mod_idx *= 6;
        }

        /* 3. clear visited array */
        memset(visited, 0, sizeof(visited));

        /* 4. mark residues of squares modulo 6^n */
        for (idx_t x = 0; x < mod_idx; ++x)
        {
            seq_t sx   = (seq_t)x;
            seq_t sq   = (sx * sx) % mod_seq;
            unsigned sq_idx = (unsigned)sq;   /* 0 ≤ sq < 6^n ≤ 46656 */
            visited[sq_idx] = 1;
        }

        /* 5. generate permutations of length n and count */
        seq_t count = 0;
        gen_permutations(n, 0, (seq_t)0, 0u, &count);

        cache[(size_t)n] = count;
        computed[(size_t)n] = true;
    }

    return cache[(size_t)n];
}

/* Recursive generation of n‑digit permutations.
   pos      – current position (0 = least significant)
   val      – current base‑6 value
   used     – bitmask of already used digits
   cnt      – pointer to counter to be incremented when valid */
static void gen_permutations(idx_t n, idx_t pos,
                             seq_t val, unsigned int used,
                             seq_t *cnt)
{
    if (pos == n)
    {
        if (visited[(unsigned)val])
            (*cnt)++;
        return;
    }

    for (unsigned d = 0; d < 6; ++d)
    {
        if (used & (1u << d))
            continue;
        seq_t new_val = val + pow6[pos] * (seq_t)d;
        gen_permutations(n, pos + 1, new_val,
                         used | (1u << d), cnt);
    }
}
