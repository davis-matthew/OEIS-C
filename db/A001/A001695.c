/*
 * A001695  a(n) = H_n(2,n) where H_n is the n-th hyperoperator.
 * https://oeis.org/A001695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = f(n,n) where f(x,y) is the Ackermann-like hyperoperator
 *            function
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t f(idx_t x, seq_t y);

#define MAX_MEMO 1024

static struct {
    idx_t x;
    seq_t y;
    seq_t val;
} memo_table[MAX_MEMO];
static int memo_count = 0;

static bool lookup_memo(idx_t x, seq_t y, seq_t *out) {
    for (int i = 0; i < memo_count; i++) {
        if (memo_table[i].x == x && memo_table[i].y == y) {
            *out = memo_table[i].val;
            return true;
        }
    }
    return false;
}

static void store_memo(idx_t x, seq_t y, seq_t val) {
    if (memo_count < MAX_MEMO) {
        memo_table[memo_count].x = x;
        memo_table[memo_count].y = y;
        memo_table[memo_count].val = val;
        memo_count++;
    }
}

static seq_t f(idx_t x, seq_t y) {
    if (x == 0) return y + 1;
    if (x == 1) return y + 2;
    if (x == 2) return y << 1;
    if (x == 3) {
        seq_t result = 1;
        for (seq_t i = 0; i < y; i++) result <<= 1;
        return result;
    }
    
    seq_t cached;
    if (lookup_memo(x, y, &cached)) return cached;
    
    seq_t result;
    if (y == 0) {
        result = 1;
    } else {
        result = f(x - 1, f(x, y - 1));
    }
    
    store_memo(x, y, result);
    return result;
}

seq_t a(idx_t n) {
    return f(n, (seq_t)n);
}
