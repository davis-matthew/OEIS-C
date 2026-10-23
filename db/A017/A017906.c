/*
 * A017906  Expansion of 1/(1 - x^12 - x^13 - ...).
 * https://oeis.org/A017906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 87, u32 n <= 157, u64 n <= 296,
 *            u128 n <= 574, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-12) + a(n-13) + … + a(0)  for n >= 12
   a(0) = 1,  a(1) = … = a(11) = 0
*/

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* internal storage : use a C++ vector when compiled as C++,
   otherwise raw malloc/realloc for C */
#ifdef __cplusplus
static std::vector<seq_t> a_vec;
static std::vector<seq_t> pref_vec;
#else
static seq_t *a_arr = NULL;
static seq_t *pref_arr = NULL;
#endif
static size_t cache_len = 0;   /* number of entries that are filled */
static size_t cache_cap = 0;   /* current capacity of the arrays */

/* helper macros for accessing the storage */
#ifdef __cplusplus
#define A_VAL(i) a_vec[(size_t)(i)]
#define PREF_VAL(i) pref_vec[(size_t)(i)]
#else
#define A_VAL(i) a_arr[(size_t)(i)]
#define PREF_VAL(i) pref_arr[(size_t)(i)]
#endif

/* enlarge the storage to accommodate at least n+1 entries */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_len)
        return;

    /* determine new capacity – grow geometrically */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed)
            new_cap = needed;
        /* reallocate */
#ifdef __cplusplus
        a_vec.resize(new_cap);
        pref_vec.resize(new_cap);
#else
        a_arr = realloc(a_arr, new_cap * sizeof(seq_t));
        pref_arr = realloc(pref_arr, new_cap * sizeof(seq_t));
#endif
        cache_cap = new_cap;
    }

    /* fill missing entries (from cache_len up to needed-1) */
    for (size_t i = cache_len; i < needed; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i < 12) {
            val = (seq_t)0;
        } else {
            val = PREF_VAL(i - 12);
        }
        A_VAL(i) = val;
        /* compute prefix sum */
        if (i == 0)
            PREF_VAL(i) = val;
        else
            PREF_VAL(i) = PREF_VAL(i - 1) + val;
    }
    cache_len = needed;
}

extern seq_t a(idx_t n)
{
    ensure_cache(n);
    return A_VAL((size_t)n);
}
