/*
 * A078869  Number of n-tuples with elements in {2,4,6} which can occur as the
 *          differences between n+1 consecutive primes > n+1. (Values of
 *          a(11), ..., a(18) are conjectured to be correct, but are only
 *          known to be upper bounds.)
 * https://oeis.org/A078869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A078869: sequence a(n) counts n-tuples of {2,4,6} which can occur
   as the differences between n+1 consecutive primes > n+1.
   A tuple is valid iff the partial sums 0,a1,a1+a2,…,a1+…+an
   do not contain a complete residue system (mod p) for any prime p.   */
#include <stdint.h>

static const int SMALL_PRIMES[8] = {2,3,5,7,11,13,17,19};

/* Full‐residue masks for the primes above (bits 0…p−1 set).  */
static const uint32_t FULL_MASKS[8] = {
    0b11,                     /* 2  :  0,1               */
    0b111,                    /* 3  :  0,1,2             */
    0b11111,                  /* 5  :  0,1,2,3,4         */
    0b1111111,                /* 7  :  0…6               */
    0b11111111111,            /* 11 :  0…10              */
    0b1111111111111,          /* 13 :  0…12              */
    0b11111111111111111,      /* 17 :  0…16              */
    0b1111111111111111111     /* 19 :  0…18              */
};

/* Cache for the first 18 values (a(1) … a(18)).  */
static seq_t CACHE[20];
static int INITIALIZED = 0;

/* Recursive depth‑first search counting valid tuples.  */
static void dfs_count(int level, int target, int sum,
                      const uint32_t *mask, seq_t *cnt)
{
    if (level == target) {
        (*cnt)++;
        return;
    }
    static const int DELTAS[3] = {2,4,6};
    for (int d = 0; d < 3; ++d) {
        int new_sum = sum + DELTAS[d];
        uint32_t new_mask[8];
        int prune = 0;
        for (int j = 0; j < 8; ++j) {
            int p = SMALL_PRIMES[j];
            int r = new_sum % p;              /* residue modulo p */
            new_mask[j] = mask[j] | (1u << r);
            if (new_mask[j] == FULL_MASKS[j]) { /* full residue set reached */
                prune = 1;
                break;
            }
        }
        if (prune) continue;
        dfs_count(level + 1, target, new_sum, new_mask, cnt);
    }
}

/* Pre‑compute all values for n = 1 … 18.  */
static void compute_all(void)
{
    uint32_t init_mask[8];
    /* The partial sum 0 contributes residue 0 for every modulus. */
    for (int j = 0; j < 8; ++j) init_mask[j] = 1u;
    for (int n = 1; n <= 18; ++n) {
        seq_t c = 0;
        dfs_count(0, n, 0, init_mask, &c);
        CACHE[n] = c;
    }
    /* a(19) and beyond are 0 by definition.  */
    INITIALIZED = 1;
}

seq_t a(idx_t n)
{
    if (!INITIALIZED) compute_all();
    if (n < 1 || n >= 19) return 0;
    return CACHE[(int)n];
}
