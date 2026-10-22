/*
 * A003644  Discriminants of the known imaginary quadratic fields with 1 class
 *          per genus (a finite sequence).
 * https://oeis.org/A003644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, nice
 * Formula:   search over discriminants -D (D <= 10000): a(n) = n-th D such
 *            that -D is a fundamental discriminant and every reduced
 *            primitive binary quadratic form of discriminant -D is ambiguous
 *            (b=0, b=a or a=c), i.e. the class group is an elementary 2-group
 *            (one class per genus). The sequence is finite with...
 * Range:     u8 n <= 30, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   a003644_fundamental, a003644_gcd, a003644_one_class_per_genus,
 *            a003644_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/a003644_fundamental.h"
#include "../utils/a003644_gcd.h"
#include "../utils/a003644_one_class_per_genus.h"
#include "../utils/a003644_squarefree.h"

static long a003644_terms[128];
static int a003644_count = -1;

seq_t a(idx_t n)
{
    if (a003644_count < 0) {
        long D;
        int cnt = 0;
        for (D = 3; D <= 10000 && cnt < 128; D++)
            if (a003644_fundamental(D) && a003644_one_class_per_genus(D))
                a003644_terms[cnt++] = D;
        a003644_count = cnt;
    }
    if (n < 1 || n > (idx_t)a003644_count) return (seq_t)0;
    return (seq_t)a003644_terms[n - 1];
}
