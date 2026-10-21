/*
 * A206172  Number of 4X(n+1) 0..3 arrays with every 2X2 subblock in a row
 *          having an equal number of equal diagonal or equal antidiagonal
 *          elements, adjacent rows differing in this number, and new values
 *          0..3 introduced in row major order.
 * https://oeis.org/A206172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = 69*a(n-1) -1494*a(n-2) +9190*a(n-3) +49137*a(n-4) -662493*a(n-5) +655164*a(n-6) +11804292*a(n-7) -31347567*a(n-8) -49939173*a(n-9) +213341850*a(n-10) -92654442*a(n-11) -92175489*a(n-12) +40920957*a(n-13);
   Initial values: a(1)=870, a(2)=17142, a(3)=463878, a(4)=13899222, a(5)=431954406, a(6)=13703317686, a(7)=440473683078, a(8)=14283616280598, a(9)=465930004254822, a(10)=15258594360495222, a(11)=501007066854539526, a(12)=16478709230048972886, a(13)=542620670913354491622 */

#include <stdbool.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t base[13];

    if (!initialized) {
        static const char *base_str[13] = {
            "870",
            "17142",
            "463878",
            "13899222",
            "431954406",
            "13703317686",
            "440473683078",
            "14283616280598",
            "465930004254822",
            "15258594360495222",
            "501007066854539526",
            "16478709230048972886",
            "542620670913354491622"
        };
        for (int i = 0; i < 13; ++i) {
            base[i] = seq_from_str(base_str[i]);
        }
        initialized = true;
    }

    if (n <= 13) {
        return base[n - 1];
    }

    seq_t cur[13];
    for (int i = 0; i < 13; ++i) {
        cur[i] = base[i];
    }

    for (idx_t k = 14; k <= n; ++k) {
        seq_t next = 0;
        next += (seq_t)69 * cur[12];
        next -= (seq_t)1494 * cur[11];
        next += (seq_t)9190 * cur[10];
        next += (seq_t)49137 * cur[9];
        next -= (seq_t)662493 * cur[8];
        next += (seq_t)655164 * cur[7];
        next += (seq_t)11804292 * cur[6];
        next -= (seq_t)31347567 * cur[5];
        next -= (seq_t)49939173 * cur[4];
        next += (seq_t)213341850 * cur[3];
        next -= (seq_t)92654442 * cur[2];
        next -= (seq_t)92175489 * cur[1];
        next += (seq_t)40920957 * cur[0];

        for (int j = 0; j < 12; ++j) {
            cur[j] = cur[j + 1];
        }
        cur[12] = next;
    }

    return cur[12];
}
