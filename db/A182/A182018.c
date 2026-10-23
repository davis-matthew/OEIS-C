/*
 * A182018  Row square-sums of triangle A210658.
 * https://oeis.org/A182018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (Sum_{i=k..n} Catalan(i))^2 where Catalan(i)
 *            = A000108(i)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Allocate an array of seq_t of given length.
   Construct each element if compiling as C++ (necessary for
   arbitrary‑precision types).  */
static seq_t* alloc_seq_array(size_t len) {
    seq_t* arr = (seq_t*)malloc(len * sizeof(seq_t));
    #ifdef __cplusplus
    for (size_t i = 0; i < len; ++i) {
        new (arr + i) seq_t();   /* placement new */
    }
    #endif
    return arr;
}

/* Free an array allocated by alloc_seq_array.
   Destruct each element if compiling as C++ (necessary for
   arbitrary‑precision types).  */
static void free_seq_array(seq_t* arr, size_t len) {
    if (!arr) return;
    #ifdef __cplusplus
    for (size_t i = 0; i < len; ++i) {
        (arr + i)->~seq_t();    /* explicit destructor call */
    }
    #endif
    free(arr);
}

/* Compute Catalan numbers up to n and use them to obtain the
   requested sequence value.  All arithmetic is performed with
   seq_t so that the result is correct for any supported type.  */
seq_t a(idx_t n) {
    /* Handle trivial case immediately (also works for n = 0). */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* Allocate array for Catalan numbers */
    size_t len = (size_t)n + 1;
    seq_t* cat = alloc_seq_array(len);
    cat[0] = (seq_t)1;
    cat[1] = (seq_t)1;

    /* Compute Catalan(n) iteratively using the recurrence:
       C_{k} = C_{k-1} * 2*(2k-1) / (k+1) */
    for (idx_t k = 2; k <= n; ++k) {
        seq_t num = cat[k - 1];
        num *= ( (seq_t)2 * (seq_t)(2 * k - 1) );
        seq_t denom = (seq_t)(k + 1);
        cat[k] = num / denom;   /* division is exact */
    }

    /* Compute the required sums using a single pass from the
       largest index downward. */
    seq_t suffix = (seq_t)0;
    seq_t result = (seq_t)0;
    for (idx_t i = n; ; --i) {
        suffix += cat[i];
        result += suffix * suffix;
        if (i == (idx_t)0) break;
    }

    /* Clean up */
    free_seq_array(cat, len);
    return result;
}
