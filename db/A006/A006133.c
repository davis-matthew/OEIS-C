/*
 * A006133  Related to representations as sums of Fibonacci numbers.
 * https://oeis.org/A006133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003231(x) = A003234(x) for x = A006132(n); computed by
 *            search: A003231(x) = floor(x*(5+sqrt(5))/2) = A000201(x) + 2x,
 *            and A003234 lists the m with A003231(A001950(m)) =
 *            A001950(A003231(m)) - 1; walk A003234 in order and keep the
 *            values where its j-th term equals A003231(j)
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   a6133_c, a6133_hi, a6133_in3234, a6133_isqrt, a6133_lo
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/a6133_c.h"
#include "../utils/a6133_hi.h"
#include "../utils/a6133_in3234.h"
#include "../utils/a6133_isqrt.h"
#include "../utils/a6133_lo.h"

static uint64_t *a6133_res = NULL;
static idx_t a6133_cnt = 0, a6133_cap = 0;
static uint64_t a6133_m = 0, a6133_j = 0;

seq_t a(idx_t n)
{
    while (a6133_cnt < n) {
        a6133_m++;
        if (a6133_in3234(a6133_m)) {
            a6133_j++;
            if (a6133_c(a6133_j) == a6133_m) {
                if (a6133_cnt == a6133_cap) {
                    idx_t nc = a6133_cap ? 2 * a6133_cap : 1024;
                    a6133_res = (uint64_t *)realloc(a6133_res, nc * sizeof(uint64_t));
                    a6133_cap = nc;
                }
                a6133_res[a6133_cnt++] = a6133_m;
            }
        }
    }
    return (seq_t)a6133_res[n - 1];
}
