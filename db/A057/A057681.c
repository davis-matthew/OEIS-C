/*
 * A057681  a(n) = Sum_{j=0..floor(n/3)} (-1)^j*binomial(n,3*j).
 * https://oeis.org/A057681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5)
 *            with initial values a(0)=1, a(1)=1, a(2)=1, a(3)=0, a(4)=-3,
 *            a(5)=-9. The recurrence holds for all n ≥ 6
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 40, i64 n <= 81, i128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
  
/* The sequence entry function.  */
seq_t a(idx_t n) {
    /* Handle the first six terms explicitly.  */
    switch (n) {
        case 0: return 1;
        case 1: return 1;
        case 2: return 1;
        case 3: return 0;
        case 4: return -3;
        case 5: return -9;
        default: break;
    }

    /* Static storage for computed terms.  */
    static seq_t *table = NULL;
    static idx_t table_cap = 0;
    static idx_t table_size = 0;

    /* Initialise the table on first use, storing a(0) … a(5).  */
    if (table == NULL) {
        table_cap = 8;                  /* initial capacity (at least 6) */
        table = (seq_t*)malloc(table_cap * sizeof(seq_t));
        if (table == NULL) return 0;    /* allocation failure, unlikely */
        table[0] = 1;
        table[1] = 1;
        table[2] = 1;
        table[3] = 0;
        table[4] = (seq_t)-3;
        table[5] = (seq_t)-9;
        table_size = 6;
    }

    /* Extend the table if required.  */
    if (n < table_size) {
        return table[n];
    }
    if (n >= table_cap) {
        /* Double capacity until it exceeds n.  */
        idx_t new_cap = table_cap;
        while (new_cap <= n) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t*)realloc(table, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;      /* allocation failure */
        table = tmp;
        table_cap = new_cap;
    }

    /* Compute the sequence up to n using the recurrence.  */
    for (idx_t i = table_size; i <= n; ++i) {
        seq_t ai = 6 * table[i-1]
                 - 15 * table[i-2]
                 + 20 * table[i-3]
                 - 15 * table[i-4]
                 + 6 * table[i-5];
        table[i] = ai;
    }
    table_size = n + 1;

    return table[n];
}
