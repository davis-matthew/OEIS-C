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
 * Helpers:   count_triangles_perimeter, find_middle_side
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
#include "../utils/count_triangles_perimeter.h"
#include "../utils/find_middle_side.h"

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
