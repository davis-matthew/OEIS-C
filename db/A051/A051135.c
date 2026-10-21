/*
 * A051135  a(n) = number of times n appears in the Hofstadter-Conway $10000
 *          sequence A004001.
 * https://oeis.org/A051135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 202, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of times n occurs in the Hofstadter–Conway 10000 sequence A004001 */
#include <stdlib.h>
#include <string.h>

/* static state shared across calls */
static seq_t *seq = NULL;          /* sequence values, 1-indexed */
static size_t seq_cap = 0;         /* allocated capacity of seq   */
static size_t seq_len = 0;         /* highest index filled in seq */

static seq_t *freq = NULL;        /* frequency table for values seen */
static size_t freq_cap = 0;        /* allocated capacity of freq    */

static size_t max_val_seen = 0;    /* largest value produced so far */

/* ensure seq has at least 'need' capacity (indices 0..need-1) */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (seq == NULL) abort();
    /* zero-initialize new part */
    memset(seq + seq_cap, 0, (new_cap - seq_cap) * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ensure freq has at least 'need' capacity (indices 0..need-1) */
static void ensure_freq_cap(size_t need)
{
    if (freq_cap >= need) return;
    size_t new_cap = freq_cap ? freq_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    freq = (seq_t *)realloc(freq, new_cap * sizeof(seq_t));
    if (freq == NULL) abort();
    memset(freq + freq_cap, 0, (new_cap - freq_cap) * sizeof(seq_t));
    freq_cap = new_cap;
}

/* initialise the static tables on first use */
static void init_tables(void)
{
    if (seq != NULL) return;       /* already initialised */

    /* allocate initial space for seq and freq */
    seq_cap = 8;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    memset(seq, 0, seq_cap * sizeof(seq_t));

    freq_cap = 8;
    freq = (seq_t *)malloc(freq_cap * sizeof(seq_t));
    memset(freq, 0, freq_cap * sizeof(seq_t));

    /* seed the Hofstadter–Conway sequence */
    seq[1] = 1;
    seq[2] = 1;
    seq_len = 2;

    /* record frequencies of the initial terms */
    ensure_freq_cap(2);
    freq[1] = 2;                  /* two occurrences of 1 */
    max_val_seen = 1;
}

/* Main function: returns the frequency of n in the HC 10000 sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* below offset */

    init_tables();

    /* If we have already generated beyond n, we can return immediately */
    if ((size_t)n < max_val_seen) {
        /* freq array is big enough because max_val_seen > n implies freq cap > n+1 */
        return freq[(size_t)n];
    }

    /* Ensure the frequency array covers up to n */
    ensure_freq_cap((size_t)n + 1);

    /* Generate terms until we see a value > n (then all n's have been counted) */
    while (max_val_seen <= (size_t)n) {
        size_t next_index = seq_len + 1;
        ensure_seq_cap(next_index + 1);

        seq_t prev_val = seq[seq_len];
        size_t idx1 = (size_t)prev_val;
        size_t idx2 = next_index - idx1;
        seq_t next_val = seq[idx1] + seq[idx2];

        seq[next_index] = next_val;
        seq_len = next_index;

        ensure_freq_cap((size_t)next_val + 1);
        freq[(size_t)next_val]++;

        if (next_val > max_val_seen) max_val_seen = (size_t)next_val;
        /* loop condition will terminate when max_val_seen > n */
    }

    return freq[(size_t)n];
}
