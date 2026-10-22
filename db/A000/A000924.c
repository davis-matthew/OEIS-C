/*
 * A000924  Class number of Q(sqrt(-n)), n squarefree.
 * https://oeis.org/A000924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = h(-m), where m is the n-th squarefree number (A005117)
 *            and h is the class number of Q(sqrt(-m)). The code finds m by
 *            search, then counts the reduced forms (a,b,c) with 4ac - b^2 =
 *            |D|, where D = -m if m == 3 (mod 4) and D = -4m otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a924_class_number, a924_nth_sqfree, a924_sqfree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/a924_class_number.h"
#include "../utils/a924_nth_sqfree.h"
#include "../utils/a924_sqfree.h"

seq_t a(idx_t n) {
    return a924_class_number(a924_nth_sqfree(n));
}
