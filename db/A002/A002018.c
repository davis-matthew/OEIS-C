/*
 * A002018  From a distribution problem.
 * https://oeis.org/A002018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n * (n*a(n-1) - (n-1)*(n-2)/2 * a(n-2)) for n > 1, a(0) =
 *            1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

typedef struct {
    idx_t index;
    seq_t value;
} MemoEntry;

static MemoEntry *memo_table = NULL;
static size_t memo_count = 0;
static size_t memo_capacity = 0;

static int find_memo(idx_t n, seq_t *out) {
    for (size_t i = 0; i < memo_count; i++) {
        if (memo_table[i].index == n) {
            *out = memo_table[i].value;
            return 1;
        }
    }
    return 0;
}

static void add_memo(idx_t n, seq_t value) {
    if (memo_count >= memo_capacity) {
        memo_capacity = memo_capacity ? memo_capacity * 2 : 16;
        MemoEntry *new_table = (MemoEntry *)malloc(memo_capacity * sizeof(MemoEntry));
        if (!new_table) return;
        for (size_t i = 0; i < memo_count; i++) {
            new_table[i] = memo_table[i];
        }
        if (memo_table) free(memo_table);
        memo_table = new_table;
    }
    memo_table[memo_count].index = n;
    memo_table[memo_count].value = value;
    memo_count++;
}

seq_t a(idx_t n) {
    seq_t cached;
    if (find_memo(n, &cached)) {
        return cached;
    }
    
    seq_t result;
    if (n == 0 || n == 1) {
        result = 1;
    } else {
        seq_t a_n_minus_1 = a(n - 1);
        seq_t a_n_minus_2 = a(n - 2);
        
        seq_t n_seq = (seq_t)n;
        seq_t term1 = n_seq * a_n_minus_1;
        
        seq_t n_minus_1 = n_seq - 1;
        seq_t n_minus_2 = n_seq - 2;
        seq_t product = n_minus_1 * n_minus_2;
        seq_t term2 = (product / 2) * a_n_minus_2;
        
        result = n_seq * (term1 - term2);
    }
    
    add_memo(n, result);
    return result;
}
