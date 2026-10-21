/*
 * A081829  Numbers k such that the k-th term of Hofstadter Q-sequence is >
 *          (k+1)-th term.
 * https://oeis.org/A081829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th index k such that the Hofstadter Q‑sequence
 *            satisfies Q(k) > Q(k+1). Q(1) = Q(2) = 1, Q(m) = Q(m−Q(m−1)) +
 *            Q(m−Q(m−2)) for m > 2. (The code produces these k’s by scanning
 *            the Q‑sequence.)
 * Range:     u8 n <= 16, u16 n <= 8, u32 n <= 8, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper to grow a dynamic array of seq_t elements */
static void grow_seq(void **arr, size_t *cap, size_t need) {
    if (*cap < need) {
        size_t newcap = (*cap) ? (*cap << 1) : 8;
        while (newcap < need) newcap <<= 1;
        *arr = realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/* Dynamic cache for the Q‑sequence (1‑based indexing). */
static seq_t *Q = NULL;
static idx_t Qsize = 0;   /* number of valid entries (max index computed) */
static size_t Qcap = 0;   /* allocated capacity */

/* Dynamic cache for the list of k’s (0‑based indexing). */
static idx_t *Klist = NULL;
static size_t Ksize = 0;  /* number of k’s found */
static size_t Kcap = 0;   /* allocated capacity */

/* Next index to test for being a member of the list. */
static idx_t test_idx = 1;

/* Ensure that Q[1..N] are computed. */
static void ensure_Q_upto(idx_t N) {
    if (N <= Qsize) return;
    grow_seq((void**)&Q, &Qcap, (size_t)N + 1);  /* +1 for 1‑based */
    /* initialise base cases if needed */
    if (Qsize == 0) {
        Q[1] = 1;
        Q[2] = 1;
        Qsize = 2;
    }
    for (idx_t i = Qsize + 1; i <= N; ++i) {
        /* Q(i) depends on Q(i-Q(i-1)) and Q(i-Q(i-2)) which are < i */
        seq_t a = Q[i - (idx_t)Q[i-1]];
        seq_t b = Q[i - (idx_t)Q[i-2]];
        Q[i] = a + b;
    }
    Qsize = N;
}

/* Generate list of k’s until it contains at least n entries. */
static void grow_Klist_to(size_t n_needed) {
    while (Ksize < n_needed) {
        ++test_idx;                 /* move to next candidate index */
        ensure_Q_upto(test_idx + 1);/* need Q(test_idx) and Q(test_idx+1) */
        if (Q[test_idx] > Q[test_idx + 1]) {
            grow_seq((void**)&Klist, &Kcap, Ksize + 1);
            Klist[Ksize++] = test_idx;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for offset 1, but handle gracefully */
    grow_Klist_to((size_t)n);   /* ensure we have at least n k’s */
    return (seq_t)Klist[n-1];   /* nth element (1‑based) */
}
