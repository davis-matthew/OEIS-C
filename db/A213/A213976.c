/*
 * A213976  a(n) = n-th term of A106750 reversed.
 * https://oeis.org/A213976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=2, a(1)=1,
   a(n)=concat(a(n-2),a(n-1)) for n>=2
   (the sequence is the reversed Fibonacci word over {1,2}) */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* Compute lengths of the strings up to n */
    size_t m = (size_t)n + 1;                 /* number of sequence elements */
    size_t *len = (size_t *)malloc(m * sizeof(size_t));
    if (!len) return 0;
    len[0] = 1;  /* "2" */
    if (n >= 1) len[1] = 1;  /* "1" */
    for (size_t i = 2; i <= (size_t)n; ++i)
        len[i] = len[i-1] + len[i-2];

    size_t total = len[(size_t)n];             /* total length of a(n) */

    /* Allocate buffer for the decimal digits */
    char *buf = (char *)malloc(total + 1);      /* +1 for NUL terminator */
    if (!buf) { free(len); return 0; }
    buf[total] = '\0';

    /* Stack for iterative traversal: store index and starting position */
    size_t stack_size = (size_t)n + 1;
    size_t *stack_idx = (size_t *)malloc(stack_size * sizeof(size_t));
    size_t *stack_pos = (size_t *)malloc(stack_size * sizeof(size_t));
    if (!stack_idx || !stack_pos) { free(len); free(buf); free(stack_idx); free(stack_pos); return 0; }

    size_t sp = 0;  /* stack pointer */
    /* push initial state (n, 0) */
    stack_idx[sp] = (size_t)n; stack_pos[sp] = 0; ++sp;

    while (sp > 0) {
        --sp;
        size_t cur = stack_idx[sp];
        size_t pos = stack_pos[sp];

        if (cur == 0) {          /* base case a(0) = "2" */
            buf[pos] = '2';
            continue;
        }
        if (cur == 1) {          /* base case a(1) = "1" */
            buf[pos] = '1';
            continue;
        }

        /* For n >= 2: a(n) = a(n-2) followed by a(n-1) */
        /* Push right part first, so that left is processed first */
        stack_idx[sp] = cur - 1;          /* a(n-1) */
        stack_pos[sp] = pos + len[cur-2];
        ++sp;
        stack_idx[sp] = cur - 2;          /* a(n-2) */
        stack_pos[sp] = pos;
        ++sp;
    }

    /* Convert the string to seq_t */
    seq_t result = seq_from_str(buf);

    /* Clean up */
    free(len);
    free(buf);
    free(stack_idx);
    free(stack_pos);

    return result;
}
