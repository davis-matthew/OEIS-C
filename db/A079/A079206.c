/*
 * A079206  Numbers n in which the first K digits of n form an integer
 *          divisible by the K-th prime, for K = 1, 2, ..., M, where M is the
 *          number of digits in n.
 * https://oeis.org/A079206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 17, u32 n <= 194, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079206: the n‑th smallest positive integer whose first
   k digits (for each k from 1 up to the number of digits) form an
   integer divisible by the k‑th prime (2,3,5,…).  The function
   generates all such numbers up to 10 digits – the largest known term
   has 10 digits – stores them in an array, sorts the array, and
   returns the (n‑1)-th element (offset 1).  If n exceeds the
   available terms, 0 is returned. */

#include <stdbool.h>
#include <stdlib.h>

/* The build supplies the typedefs:
       typedef uint64_t idx_t;
       typedef … seq_t;   // uint8_t, uint16_t, uint32_t, uint64_t,
       // uint128 or an arbitrary‑precision class
*/

/* First ten primes – enough for the longest term (10 digits). */
static const seq_t PRIMES[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

#define MAX_TERMS 250
static seq_t  SEQ[MAX_TERMS];
static size_t SEQ_LEN = 0;
static bool   SEQ_INITIALIZED = false;

/* Compare function for qsort (works for all seq_t supporting < >). */
static int compare_seq(const void *a, const void *b) {
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return 1;
    return 0;
}

/* Recursive DFS that builds valid prefixes. */
static void generate(seq_t value, int len) {
    /* Store a complete number (len>0). */
    if (len > 0) {
        SEQ[SEQ_LEN++] = value;
        if (SEQ_LEN >= MAX_TERMS) return;   /* safety guard */
    }

    /* Stop when we already have 10 digits – no larger terms are known. */
    if (len >= 10) return;

    if (len == 0) {
        /* First digit must be non‑zero and make the prefix (single digit)
           divisible by the first prime (2), i.e. must be even. */
        for (int d = 2; d <= 8; d += 2) {    /* 2,4,6,8 */
            seq_t next = value * 10 + d;
            generate(next, len + 1);
        }
    } else {
        /* For subsequent digits, try all 0–9 and keep only those prefixes
           that become divisible by the next prime. */
        for (int d = 0; d <= 9; ++d) {
            seq_t next = value * 10 + d;
            int newlen = len + 1;
            if (next % PRIMES[newlen - 1] == 0) {
                generate(next, newlen);
            }
        }
    }
}

/* Build the complete list once. */
static void build_sequence(void) {
    SEQ_LEN = 0;
    generate((seq_t)0, 0);

    /* Sort the array in increasing order. */
    qsort(SEQ, SEQ_LEN, sizeof(seq_t), compare_seq);

    SEQ_INITIALIZED = true;
}

/* Public interface required by the framework. */
seq_t a(idx_t n) {
    if (!SEQ_INITIALIZED) build_sequence();

    if (n == 0)            return (seq_t)0;          /* offset is 1 */
    if ((size_t)n <= SEQ_LEN) return SEQ[(size_t)n - 1];
    return (seq_t)0;       /* terms beyond the known 200 are undefined */
}
