/*
 * A039850  Number of partitions satisfying cn(0,5) + cn(1,5) <= 1 and cn(0,5)
 *          + cn(4,5) <= 1.
 * https://oeis.org/A039850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Computation of OEIS A039850.
   a(n) counts partitions of n such that
   cn(0,5)+cn(1,5) ≤ 1 and cn(0,5)+cn(4,5) ≤ 1.
   The implementation follows the description above:
   partitions are built from unrestricted parts ≡2,3 (mod 5),
   optionally with at most one part ≡0, at most one part ≡1 and at most
   one part ≡4, with the restrictions that a part ≡0 cannot co‑occur
   with any other special part, while parts ≡1 and ≡4 may co‑occur. */

/* --- Static cache ----------------------------------------------------- */
static seq_t *cache_f = NULL;   /* partitions using only 2 or 3 mod 5 parts  */
static seq_t *cache_a = NULL;   /* final sequence values                    */
static idx_t max_computed = 0;  /* highest index cached so far              */

/* --- Recompute everything up to new_n (inclusive) --------------------- */
static void recompute_upto(idx_t new_n)
{
    /* Allocate new arrays */
    seq_t *new_f = (seq_t *)malloc(sizeof(seq_t) * (new_n + 1));
    seq_t *new_a = (seq_t *)malloc(sizeof(seq_t) * (new_n + 1));

    /* Initialise to zero */
    for (idx_t i = 0; i <= new_n; ++i) {
        new_f[i] = (seq_t)0;
        new_a[i] = (seq_t)0;
    }
    new_f[0] = (seq_t)1;  /* empty partition */

    /* Compute f: partitions with parts ≡2 or 3 mod 5 */
    for (idx_t part = 2; part <= new_n; part += 5) {          /* 5k+2 */
        for (idx_t i = part; i <= new_n; ++i) {
            new_f[i] += new_f[i - part];
        }
    }
    for (idx_t part = 3; part <= new_n; part += 5) {          /* 5k+3 */
        for (idx_t i = part; i <= new_n; ++i) {
            new_f[i] += new_f[i - part];
        }
    }

    /* Initialise a[i] with the case of no special part */
    for (idx_t i = 0; i <= new_n; ++i) {
        new_a[i] = new_f[i];
    }

    /* Add one part ≡0 (size 5k) */
    for (idx_t s = 5; s <= new_n; s += 5) {           /* s = 5k  */
        for (idx_t i = s; i <= new_n; ++i) {
            new_a[i] += new_f[i - s];
        }
    }

    /* Add one part ≡1 (size 5k+1) */
    for (idx_t s = 1; s <= new_n; s += 5) {           /* s = 5k+1  */
        for (idx_t i = s; i <= new_n; ++i) {
            new_a[i] += new_f[i - s];
        }
    }

    /* Add one part ≡4 (size 5k+4) */
    for (idx_t s = 4; s <= new_n; s += 5) {           /* s = 5k+4  */
        for (idx_t i = s; i <= new_n; ++i) {
            new_a[i] += new_f[i - s];
        }
    }

    /* Add one part ≡1 and one part ≡4 together (sizes 5k+1 and 5l+4) */
    for (idx_t s1 = 1; s1 <= new_n; s1 += 5) {
        for (idx_t s4 = 4; s4 <= new_n - s1; s4 += 5) {
            idx_t sum = s1 + s4;                     /* total size of the two special parts */
            for (idx_t i = sum; i <= new_n; ++i) {
                new_a[i] += new_f[i - sum];
            }
        }
    }

    /* Replace old cache with new one */
    if (cache_f) free(cache_f);
    if (cache_a) free(cache_a);
    cache_f = new_f;
    cache_a = new_a;
    max_computed = new_n;
}

/* --- Public interface ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;                 /* below offset */
    if (max_computed < n) recompute_upto(n);
    return cache_a[n];
}
