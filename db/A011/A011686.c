/*
 * A011686  A binary m-sequence: expansion of reciprocal of x^7 + x^6 + 1.
 * https://oeis.org/A011686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 126, u32 n <= 126, u64 n <= 126,
 *            u128 n <= 126, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=0,a(5)=0,a(6)=1;
   for n≥7, a(n)=a(n-7) XOR a(n-6).  The sequence is 127‑periodic. */
#include <stdbool.h>

seq_t a(idx_t n){
    static seq_t cache[127];
    static bool init = false;
    if (!init) {
        cache[0] = 0; cache[1] = 0; cache[2] = 0; cache[3] = 0;
        cache[4] = 0; cache[5] = 0; cache[6] = 1;
        for (idx_t i = 7; i < 127; ++i) {
            cache[i] = cache[i-7] ^ cache[i-6];
        }
        init = true;
    }
    return cache[(idx_t)(n % 127)];
}
