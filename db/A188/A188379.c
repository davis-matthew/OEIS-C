/*
 * A188379  a(n) = A002526(n+1) - A002527(n+1).
 * https://oeis.org/A188379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A188379
    a(n) = A002526(n+1) – A002527(n+1)
    Recurrence (valid for n ≥ 14):
    a[n] = 2*a[n-1] + 2*a[n-2] +10*a[n-4] + 8*a[n-5] – 2*a[n-6]
           –16*a[n-7] –10*a[n-8] –2*a[n-9] + 4*a[n-10] + 2*a[n-11]
           +2*a[n-13] + a[n-14]
    Initial values (n = 0 … 13):
    0, 0, 0, 6, 18, 46, 115, 374, 1204, 3752, 11300, 34324, 105124, 322989
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial 14 terms */
    static const seq_t init[14] = {
        0ULL, 0ULL, 0ULL, 6ULL, 18ULL, 46ULL, 115ULL, 374ULL,
        1204ULL, 3752ULL, 11300ULL, 34324ULL, 105124ULL, 322989ULL
    };

    if (n < 14) return init[n];

    seq_t buf[14];
    for (int i = 0; i < 14; ++i) buf[i] = init[i];

    for (idx_t k = 14; k <= n; ++k) {
        unsigned __int128 pos = 0;
        unsigned __int128 neg = 0;

        /* indices relative to current k */
        int i1  = (int)((k - 1) % 14);
        int i2  = (int)((k - 2) % 14);
        int i4  = (int)((k - 4) % 14);
        int i5  = (int)((k - 5) % 14);
        int i6  = (int)((k - 6) % 14);
        int i7  = (int)((k - 7) % 14);
        int i8  = (int)((k - 8) % 14);
        int i9  = (int)((k - 9) % 14);
        int i10 = (int)((k -10) % 14);
        int i11 = (int)((k -11) % 14);
        int i13 = (int)((k -13) % 14);
        int i14 = (int)((k -14) % 14);

        pos += (unsigned __int128)buf[i1] * 2;
        pos += (unsigned __int128)buf[i2] * 2;
        pos += (unsigned __int128)buf[i4] * 10;
        pos += (unsigned __int128)buf[i5] * 8;
        pos += (unsigned __int128)buf[i10] * 4;
        pos += (unsigned __int128)buf[i11] * 2;
        pos += (unsigned __int128)buf[i13] * 2;
        pos += (unsigned __int128)buf[i14] * 1;

        neg += (unsigned __int128)buf[i6] * 2;
        neg += (unsigned __int128)buf[i7] * 16;
        neg += (unsigned __int128)buf[i8] * 10;
        neg += (unsigned __int128)buf[i9] * 2;

        seq_t next = (seq_t)(pos - neg);
        buf[k % 14] = next;
    }

    return buf[n % 14];
}
