/*
 * A138481  Mutually-praising pairs excluding autobiographical numbers
 *          A046043. Version 1: both numbers in a pair have at most 10 digits.
 * https://oeis.org/A138481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) is the nth smallest member of all mutually‑praising pairs
 *            (excluding autobiographic numbers), each number having at most
 *            10 digits
 * Range:     u8 none, u16 n <= 8, u32 n <= 67, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Auxiliary dynamic array for storing the sequence. */
static seq_t *seq_array  = NULL;   /* array of sequence values      */
static size_t seq_len    = 0;      /* current number of elements   */
static size_t seq_cap    = 0;      /* current capacity of seq_array */

/* Append a value to the dynamic array, reallocating if necessary. */
static void append_value(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 16 : seq_cap << 1;
        seq_t *new_ptr = (seq_t *)realloc(seq_array, new_cap * sizeof(seq_t));
        if (!new_ptr) exit(EXIT_FAILURE);   /* out of memory */
        seq_array = new_ptr;
        seq_cap   = new_cap;
    }
    seq_array[seq_len++] = val;
}

/* Compare function for qsort on seq_t values. */
static int cmp_seq_t(const void *p1, const void *p2)
{
    seq_t v1 = *(const seq_t *)p1;
    seq_t v2 = *(const seq_t *)p2;
    if (v1 < v2) return -1;
    if (v1 > v2) return  1;
    return 0;
}

/* Maximimum representable value in seq_t.  We use a 64‑bit unsigned
   integer to hold it; for seq_t types >64 bits the lower 64 bits of
   ~0 are always all 1 (i.e. 2^64‑1), which is ≥ any 10‑digit number. */
static const unsigned long long MAX_SEQ_T = (unsigned long long)~(seq_t)0;

/* Process one candidate digit sequence a[0..k-1] that is of length k
   and whose digits sum to l (=sum of a).  If a and its mutual partner
   satisfy all conditions, the numbers are added to the global array. */
static void process_candidate(const int *a, int k)
{
    int sum = 0;
    for (int i = 0; i < k; ++i) sum += a[i];
    if (sum == 0 || sum > 10) return;          /* l must be between 1 and 10 */
    int l = sum;                                 /* length of partner */

    /* Count frequency of each digit in a (X). */
    int freqX[10] = {0};
    for (int i = 0; i < k; ++i) ++freqX[a[i]];

    /* All digits used by Y must be single decimal digits. */
    for (int d = 0; d < l; ++d) {
        if (freqX[d] > 9) return;
    }

    /* Build Y's digit array b[0..l-1] and count its frequencies. */
    unsigned char b[10];
    for (int d = 0; d < l; ++d) b[d] = (unsigned char)freqX[d];

    /* Leading zero in Y is not allowed (unless Y would be the single digit 0,
       which we explicitly skip). */
    if (l > 1 && b[0] == 0) return;

    int freqY[10] = {0};
    for (int j = 0; j < l; ++j) ++freqY[b[j]];

    /* Mutual condition: a[i] must equal freqY[i] for all i < k. */
    for (int i = 0; i < k; ++i) {
        if (a[i] != freqY[i]) return;
    }

    /* Convert the digit sequences into numeric values. */
    uint64_t X_val = 0;
    for (int i = 0; i < k; ++i) X_val = X_val * 10 + a[i];
    uint64_t Y_val = 0;
    for (int j = 0; j < l; ++j) Y_val = Y_val * 10 + b[j];

    /* Skip the zero number itself. */
    if (X_val == 0 || Y_val == 0) return;

    /* Skip autobiographic pairs where the two numbers are equal. */
    if (X_val == Y_val) return;

    /* Store the numbers if they fit into seq_t. */
    if (X_val <= MAX_SEQ_T) append_value((seq_t)X_val);
    if (Y_val <= MAX_SEQ_T) append_value((seq_t)Y_val);
}

/* Recursive enumeration of all digit sequences of length k with digit sum
   <= 10.  The array \a a holds the digits as they are chosen. */
static void recurse(int k, int pos, int current_sum, int *a)
{
    if (pos == k) {
        process_candidate(a, k);
        return;
    }
    for (int d = 0; d <= 9; ++d) {
        if (pos == 0 && d == 0) continue;     /* no leading zero */
        if (current_sum + d > 10) break;        /* sum would exceed the bound */
        a[pos] = d;
        recurse(k, pos + 1, current_sum + d, a);
    }
}

/* Generate the complete sequence (once).  After execution,
   seq_array contains the sorted, deduplicated list of all values. */
static void generate_sequence(void)
{
    int a[10];   /* temporary buffer for a candidate number */
    for (int k = 1; k <= 10; ++k) {
        recurse(k, 0, 0, a);
    }

    /* Sort the values. */
    qsort(seq_array, seq_len, sizeof(seq_t), cmp_seq_t);

    /* Remove duplicates in place. */
    size_t uniq_len = 0;
    for (size_t i = 0; i < seq_len; ++i) {
        if (uniq_len == 0 || seq_array[i] != seq_array[uniq_len - 1]) {
            seq_array[uniq_len++] = seq_array[i];
        }
    }
    seq_len = uniq_len;
}

/* Public API required by the test harness. */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        generate_sequence();
        initialized = 1;
    }
    if (n == 0 || (idx_t)seq_len < n) return (seq_t)0;
    return seq_array[n - 1];
}
