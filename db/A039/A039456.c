/*
 * A039456  Numbers whose base-11 representation has the same nonzero number
 *          of 1's and 10's.
 * https://oeis.org/A039456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   search for all positive integers whose base‑11 representation
 *            contains the same nonzero number of digits equal to 1 and digits
 *            equal to 10. The sequence is produced in increasing order of the
 *            integer value. The function a(idx_t n) returns the n‑th term (n
 *            ≥ 1)
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following code is valid C11 and C++17.  It does not rely on any
   external helper functions beyond the standard header <stdlib.h>
   and <stddef.h>.

   - seq_t may be any unsigned integer type or a big‑int class that
     supports the operators + - * / % and comparison >, ==, etc.
   - idx_t is an unsigned 64‑bit integer (uint64_t).  It is used only
     for loop counters and array indices.
   - The algorithm keeps a static array of already found terms so that
     repeated calls to a(n) are O(1) after the values up to n have been
     generated.  The array is grown as needed.
*/

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;              /* array of found terms   */
static size_t cache_len = 0;             /* current number of terms*/
static size_t cache_cap = 0;             /* allocated capacity     */
static idx_t  next_test = 1;             /* next decimal value to test */

/* Append one term to the cache, growing it if necessary.  The
   implementation works for both C and C++ (the latter uses new/delete). */
static void append_to_cache(seq_t val)
{
    if (cache_len == cache_cap) {
#ifdef __cplusplus
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *newp = new seq_t[new_cap];
        for (size_t i = 0; i < cache_len; ++i) newp[i] = cache[i];
        delete[] cache;
        cache = newp;
        cache_cap = new_cap;
#else
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) abort();
        cache = newp;
        cache_cap = new_cap;
#endif
    }
    cache[cache_len++] = val;
}

/* Generate sequence terms up to at least index n (1‑based).  This
   function incrementally tests increasing decimal values, converts
   them to base‑11 and counts digits 1 and 10.  If both counts are
   equal and nonzero, the value is stored. */
static void generate_up_to(idx_t n)
{
    if (n <= cache_len) return;          /* already enough terms */

    while (cache_len < n) {
        seq_t val = (seq_t)next_test++;   /* decimal value to test */

        /* Count digits 1 and 10 in base 11. */
        size_t cnt1  = 0;
        size_t cnt10 = 0;
        seq_t tmp = val;
        while (tmp > 0) {
            unsigned digit = (unsigned)(tmp % 11);
            if (digit == 1)  ++cnt1;
            else if (digit == 10) ++cnt10;
            tmp /= 11;
        }

        if (cnt1 == cnt10 && cnt1 > 0)     /* valid term */
            append_to_cache(val);
    }
}

/* Public sequence function.  a(0) is defined as 0 for safety. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    generate_up_to(n);
    return cache[n - 1];
}
