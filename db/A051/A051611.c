/*
 * A051611  Numbers that are not the sum of 2 nonzero triangular numbers.
 * https://oeis.org/A051611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A051611: Numbers that are not the sum of 2 non‑zero triangular numbers. */
/* The program tests each integer m on demand; it keeps the already found terms in a
 * static array and expands it as necessary.  To decide if a number can be written
 * as T(a)+T(b) (with T(k)=k(k+1)/2) it simply enumerates all triangular numbers
 * up to m and checks whether the complementary value is triangular.  A value is
 * triangular iff 8*x+1 is a perfect square.  The necessary square roots are
 * computed with binary search on unsigned __int128 to avoid overflow in all
 * supported builds.  The routine works for any seq_t (up to unsigned __int128
 * or larger arbitrary‑precision types) and any idx_t (uint64_t). */
#include <stdlib.h>
#include <stdbool.h>

/* Unsigned 128‑bit intermediate type.  Works both in C and C++. */
typedef unsigned __int128 u128;

/* Square root of an unsigned __int128 number (floor). */
static inline u128 sqrt128(u128 n)
{
    u128 low = 0, high = n, ans = 0;
    while (low <= high) {
        u128 mid = low + ((high - low) >> 1);
        u128 sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* Returns true iff d is a triangular number (with d>0). */
static inline bool is_triangular(seq_t d)
{
    if (d == 0) return false;                     /* we need non‑zero */
    u128 val  = ((u128)8 * (u128)d) + 1;         /* 8*d+1 */
    u128 root = sqrt128(val);
    return root * root == val;
}

/* Returns true iff m can be expressed as the sum of two non‑zero triangular numbers. */
static inline bool has_two_triangular_sum(seq_t m)
{
    /* Enumerate T(i)=i(i+1)/2 for i>=1.  Stop when T(i)>m. */
    for (idx_t i = 1; ; ++i) {
        u128 ui   = (u128)i;
        u128 Ti128 = (ui * (ui + 1)) >> 1;       /* i*(i+1)/2   */
        if (Ti128 > (u128)m) break;
        seq_t Ti = (seq_t)Ti128;
        seq_t diff = m - Ti;
        if (diff > 0 && is_triangular(diff))
            return true;
    }
    return false;
}

/* Cached sequence.  When more terms are requested the array is grown as needed. */
static seq_t *terms = NULL;          /* array of found terms                */
static size_t size  = 0;             /* number of terms stored              */
static size_t capacity = 0;          /* current allocation size             */
static idx_t tested = 0;             /* last integer that was checked       */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but never used by OEIS tests */

    /* Grow the cache until it contains at least n terms. */
    while (size < n) {
        /* Ensure enough capacity. */
        if (capacity == 0) {
            capacity = 1;
            terms = (seq_t*)malloc(capacity * sizeof(seq_t));
        } else if (size == capacity) {
            capacity *= 2;
            terms = (seq_t*)realloc(terms, capacity * sizeof(seq_t));
        }
        /* Test the next integer. */
        ++tested;
        seq_t m = (seq_t)tested;
        if (!has_two_triangular_sum(m)) {
            terms[size++] = m;
        }
    }
    return terms[n - 1];
}
