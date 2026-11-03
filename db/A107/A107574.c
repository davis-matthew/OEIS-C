/*
 * A107574  a(n)=middle sidelength of n-th triangle listed at A107572.
 * https://oeis.org/A107574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the middle side of the n-th integer‑sided scalene
 *            triangle (a<b<c) sorted by increasing perimeter, and for equal
 *            perimeter sorted lexicographically by (a,b,c)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Validated: partial, pass, 95 terms through n = 95 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm enumerates perimeters until the n‑th triangle is found.
   For each perimeter p it counts how many triangles satisfy
          a<b<c, a+b+c = p, a+b>c.
   The count for a given first side a is
          max(0, floor((p-a-1)/2) - max(a+1, floor((p-2a)/2)+1) + 1) .
   Then it locates the specific triangle within that perimeter. */

#include <stdint.h>

static idx_t count_triangles_perimeter(idx_t p)
{
    idx_t total = 0;
    /* a ranges from 2 to floor((p-3)/3) */
    idx_t a_max = (p >= 3) ? (p - 3) / 3 : 0;
    for (idx_t a = 2; a <= a_max; ++a) {
        int64_t a64 = (int64_t)a;
        int64_t p64 = (int64_t)p;

        /* b_min from triangle inequality a+b>c -> 2(a+b) > p */
        int64_t b_min_ineq = p64 - 2*a64;
        int64_t b_min = a + 1;
        if (b_min_ineq > 0) {
            b_min = (idx_t)((b_min_ineq / 2) + 1);
            if (b_min < a + 1) b_min = a + 1;
        }

        /* b_max from c > b -> p - a - b > b */
        int64_t b_max = (p64 - a64 - 1) / 2;
        if ((int64_t)b_max < (int64_t)b_min) continue;

        total += (idx_t)(b_max - b_min + 1);
    }
    return total;
}

static seq_t find_middle_side(idx_t p, idx_t k)
{
    /* Find the k-th triangle among those with perimeter p. */
    idx_t a_max = (p >= 3) ? (p - 3) / 3 : 0;
    for (idx_t a = 2; a <= a_max; ++a) {
        int64_t a64 = (int64_t)a;
        int64_t p64 = (int64_t)p;

        int64_t b_min_ineq = p64 - 2*a64;
        idx_t b_min = a + 1;
        if (b_min_ineq > 0) {
            idx_t tmp = (idx_t)((b_min_ineq / 2) + 1);
            if (tmp > b_min) b_min = tmp;
        }

        idx_t b_max = (idx_t)((p64 - a64 - 1) / 2);
        if (b_max < b_min) continue;

        idx_t cnt = b_max - b_min + 1;
        if (k <= cnt) {
            idx_t b = b_min + (k - 1);
            return (seq_t)b;
        }
        k -= cnt;
    }
    /* Should never reach here for valid n */
    return (seq_t)0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* n>=1 but handle defensively */

    idx_t cumulative = 0;
    idx_t perimeter = 9; /* minimal perimeter for (2,3,4) */
    while (1) {
        idx_t cnt = count_triangles_perimeter(perimeter);
        if (cumulative + cnt >= n) break;
        cumulative += cnt;
        ++perimeter;
    }
    idx_t k = n - cumulative;
    return find_middle_side(perimeter, k);
}
