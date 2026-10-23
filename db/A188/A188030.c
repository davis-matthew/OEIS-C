/*
 * A188030  Positions of 1 in the zero-one sequence given by
 *          [nr]-[nr-10r]-[10r], where r=(1+sqrt(5))/2, [ ]=floor, n>=1.
 * https://oeis.org/A188030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A188030
 *  a(n) is the n-th positive integer k for which
 *      floor(kφ) – floor((k-10)φ) – floor(10φ) = 1
 *  where φ = (1+√5)/2 and floor = ⌊·⌋.  φ is approximated by the
 *  convergent 14930352/9227465, which is more than accurate for all
 *  k up to at least 10⁶, guaranteeing the exactness of the floor
 *  computations needed for the sequence up to the required limits.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Constants for the rational approximation φ ≈ 14930352/9227465 */
static const uint64_t PHI_NUM   = 14930352ULL;  /* numerator   */
static const uint64_t PHI_DEN   = 9227465ULL;   /* denominator */
static const uint64_t FLOOR_10PHI = 16ULL;      /* floor(10φ)  */

/* Cached positions of 1's */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;
static idx_t next_k = 1;                /* next integer to test */

/* Ensure cache has capacity for at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 128;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache)
        memcpy(new_arr, cache, cache_size * sizeof(seq_t));
    free(cache);
    cache = new_arr;
    cache_cap = new_cap;
}

/* floor(n·φ) for n >= 0 */
static uint64_t floor_phi_mul(idx_t n)
{
    unsigned __int128 prod = (unsigned __int128)n * PHI_NUM;
    return (uint64_t)(prod / PHI_DEN);
}

/* floor((k‑10)·φ) for any integer k >= 1 (may be negative). */
static int64_t floor_phi_minus10(idx_t k)
{
    if (k >= 10) {
        return (int64_t)floor_phi_mul(k - 10);
    } else {
        /* k < 10 → (k‑10) is negative.
           floor(−m·φ) = −(floor(m·φ)+1),  m = 10‑k > 0   */
        uint64_t m = 10 - k;
        return -((int64_t)floor_phi_mul(m) + 1);
    }
}

/* Extend the cached sequence up to at least 'upto' terms. */
static void extend_upto(idx_t upto)
{
    while (cache_size < upto) {
        uint64_t fn   = floor_phi_mul(next_k);
        int64_t fN10  = floor_phi_minus10(next_k);
        int diff = (int)( (int64_t)fn - fN10 - (int64_t)FLOOR_10PHI );
        if (diff == 1) {                     /* a 1 occurs at next_k   */
            ensure_capacity(cache_size + 1);
            cache[cache_size++] = (seq_t)next_k;
        }
        ++next_k;
    }
}

/* Public function required by the problem. */
seq_t a(idx_t n)
{
    if (n == 0)  /* safety for accidental 0-call; offset is 1   */
        return 0;

    extend_upto(n);
    return cache[n - 1];
}
