/*
 * A002574  Restricted partitions.
 * https://oeis.org/A002574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) counts compositions of n starting with p(1)=3 where p(k+1)
 *            <= 2*p(k)
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 41, u64 n <= 79, u128 n <= 155,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MEMO_SIZE 100007

typedef struct {
    idx_t rem;
    idx_t last;
    seq_t value;
} MemoEntry;

static MemoEntry memo[MEMO_SIZE];
static bool memo_used[MEMO_SIZE];

static unsigned long hash_fn(idx_t rem, idx_t last) {
    return (rem * 31 + last) % MEMO_SIZE;
}

static seq_t count_comp(idx_t rem, idx_t last);

static bool get_memo(idx_t rem, idx_t last, seq_t *out) {
    unsigned long h = hash_fn(rem, last);
    for (int i = 0; i < MEMO_SIZE; i++) {
        unsigned long pos = (h + i) % MEMO_SIZE;
        if (!memo_used[pos]) return false;
        if (memo[pos].rem == rem && memo[pos].last == last) {
            *out = memo[pos].value;
            return true;
        }
    }
    return false;
}

static void set_memo(idx_t rem, idx_t last, seq_t value) {
    unsigned long h = hash_fn(rem, last);
    for (int i = 0; i < MEMO_SIZE; i++) {
        unsigned long pos = (h + i) % MEMO_SIZE;
        if (!memo_used[pos]) {
            memo[pos].rem = rem;
            memo[pos].last = last;
            memo[pos].value = value;
            memo_used[pos] = true;
            return;
        }
    }
}

static seq_t count_comp(idx_t rem, idx_t last) {
    if (rem == 0) return 1;
    
    seq_t cached;
    if (get_memo(rem, last, &cached)) {
        return cached;
    }
    
    seq_t result = 0;
    idx_t max_next = (rem < 2 * last) ? rem : 2 * last;
    for (idx_t next = 1; next <= max_next; next++) {
        result = result + count_comp(rem - next, next);
    }
    
    set_memo(rem, last, result);
    return result;
}

seq_t a(idx_t n) {
    if (n < 3) return 0;
    
    memset(memo_used, 0, sizeof(memo_used));
    
    return count_comp(n - 3, 3);
}
