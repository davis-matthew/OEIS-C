/*
 * A204926  Least Fibonacci number f such that n divides f-g for some
 *          Fibonacci number g satisfying g < f.
 * https://oeis.org/A204926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 58, u32 n <= 192, u64 n <= 590,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A204926 : Least Fibonacci number f such that n divides f‑g for
    some Fibonacci number g with g < f.  The algorithm
    simulates the Fibonacci sequence modulo n until a value repeats
    (excluding the trivial repeat of the first two 1’s).  When a repeat
    occurs at index k, the corresponding Fibonacci number F_k is
    returned.  The implementation keeps the Fibonacci numbers in a
    big‑integer type seq_t so the result can be arbitrarily large. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Forward declaration of the Fibonacci routine supplied by the
   utility library.  It returns the k‑th Fibonacci number
   (F_1 = 1, F_2 = 1).  The index type `idx_t` is defined by the build. */
seq_t fib(idx_t n);

seq_t a(idx_t n)
{
    /* For n = 0 the function is undefined; return 0 for safety. */
    if (n == 0) return 0;

    /* Allocate a table of seen residues modulo `n`. */
    size_t sz = (size_t)n;
    bool *seen = (bool *)calloc(sz, sizeof(bool));
    if (!seen) return 0;   /* allocation failure → return 0 */

    /*  Fibonacci numbers modulo n, starting with F_1 = 1, F_2 = 1. */
    idx_t mod_prev = 1 % n;
    idx_t mod_curr = 1 % n;
    seen[mod_prev] = true;
    seen[mod_curr] = true;    /* duplicates the first entry; harmless */

    /* Fibonacci numbers themselves, using seq_t (arbitrary‑precision if
       provided by the build). */
    seq_t f_prev = 1;     /* F_1 */
    seq_t f_curr = 1;     /* F_2 */
    idx_t k = 2;          /* current index of f_curr */

    for (;;)
    {
        /* Next residue and next Fibonacci number. */
        idx_t mod_next = (mod_prev + mod_curr) % n;
        seq_t f_next = f_prev + f_curr;

        ++k;  /* new index is k = previous index + 1 */

        if (seen[mod_next])
        {
            free(seen);
            return f_next;   /* F_k with repeated residue. */
        }

        seen[mod_next] = true;
        mod_prev = mod_curr;
        mod_curr = mod_next;
        f_prev = f_curr;
        f_curr = f_next;
    }
}
